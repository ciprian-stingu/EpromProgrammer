/**
 * 
 * Eprom Programmer
 * 
 * Copyright (C) 2021-2023 Ciprian Stingu
 * ciprian stingu at gmail dot com
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 USA
 * or see <http://www.gnu.org/licenses/>
 * 
 */
//------------------------------------------------------------------------------
#include "defines.h"
#include "io.h"
#include "write.h"
#include "read.h"
//------------------------------------------------------------------------------
//write a byte to eprom
void WriteDataByte(uint8_t data)
{
#ifndef OPTIMIZE_FOR_SPEED
  digitalWrite(D0_PIN, (data & (1 << 0)));
  digitalWrite(D1_PIN, (data & (1 << 1)));
  digitalWrite(D2_PIN, (data & (1 << 2)));
  digitalWrite(D3_PIN, (data & (1 << 3)));
  digitalWrite(D4_PIN, (data & (1 << 4)));
  digitalWrite(D5_PIN, (data & (1 << 5)));
  digitalWrite(D6_PIN, (data & (1 << 6)));
  digitalWrite(D7_PIN, (data & (1 << 7)));
#else
  uint8_t portB = digitalPinToPort(D1_PIN);
  uint8_t portD = digitalPinToPort(D0_PIN);

  uint8_t maskD0 = !(data & 0x01) ? ~digitalPinToBitMask(D0_PIN) : 0xFF;
  uint8_t maskD1 = !(data & 0x02) ? ~digitalPinToBitMask(D1_PIN) : 0xFF;
  uint8_t maskD2 = !(data & 0x04) ? ~digitalPinToBitMask(D2_PIN) : 0xFF;
  uint8_t maskD3 = !(data & 0x08) ? ~digitalPinToBitMask(D3_PIN) : 0xFF;
  uint8_t maskD4 = !(data & 0x10) ? ~digitalPinToBitMask(D4_PIN) : 0xFF;
  uint8_t maskD5 = !(data & 0x20) ? ~digitalPinToBitMask(D5_PIN) : 0xFF;
  uint8_t maskD6 = !(data & 0x40) ? ~digitalPinToBitMask(D6_PIN) : 0xFF;
  uint8_t maskD7 = !(data & 0x80) ? ~digitalPinToBitMask(D7_PIN) : 0xFF;

  uint8_t maskB = 0xFF & maskD1 & maskD2;
  uint8_t maskD = 0xFF & maskD0 & maskD3 & maskD4 & maskD5 & maskD6 & maskD7;

  volatile uint8_t *outB = portOutputRegister(portB);
  volatile uint8_t *outD = portOutputRegister(portD);

  uint8_t backupSREG = SREG;
  cli();

  *outB &= maskB;
  *outD &= maskD;
  
  SREG = backupSREG;
#endif
}
//------------------------------------------------------------------------------

void StartWriteData(void)
{
  extern EPROM_TYPE EpromSelected;
  
  SetDataMode(true);

  switch(EpromSelected)
  {
    case EPROM_TYPE::X16:
    case EPROM_TYPE::C16:
      digitalWrite(NCE_PIN, LOW);
      digitalWrite(NOE_PIN, HIGH);
      break;
      
    default:
      digitalWrite(NCE_PIN, HIGH);
      digitalWrite(NOE_PIN, HIGH);
  }
}
//------------------------------------------------------------------------------

void EndWriteData(void)
{
  SetDataMode(false);

  digitalWrite(NCE_PIN, HIGH);
  digitalWrite(NOE_PIN, HIGH);
}
//------------------------------------------------------------------------------

bool WriteEprom(void)
{
  extern EPROM_TYPE EpromSelected;
  extern uint32_t EpromEndAddress;
  extern uint8_t *SerialBuffer;
  extern PRG_ALGO ProgrammingAlgorithm;

  for(uint32_t i = 0; i < EpromEndAddress; i += SERIAL_BUF_LEN)
  { 
    Serial.print(F(RESPONSE_FLAG));
    Serial.print(F(RESPONSE_BLOCK));
    Serial.println(i);
    Serial.flush();

    WaitForData();

    uint8_t count = Serial.readBytes(SerialBuffer, SERIAL_BUF_LEN);    
    if(count != SERIAL_BUF_LEN)
    {
      Serial.print(F(RESPONSE_FLAG));
      Serial.print(F(RESPONSE_ERROR));
      Serial.print(count);
      Serial.print(F(" bytes received for block 0x"));
      Serial.println(i, HEX);
      Serial.flush();
      return false;
    }

    StartWriteData();

    bool writeResult = true;
    switch(EpromSelected)
    {
      case EPROM_TYPE::X16:
      case EPROM_TYPE::C16:
        writeResult = WriteBlockX16(i);
        break;
      
      default:
        switch(ProgrammingAlgorithm)
        {
          case PRG_ALGO::FAST:
            writeResult = WriteFastBlock(i);
            break;
            
          default:
            writeResult = WriteStandardBlock(i);
        }
    }
    
    if(!writeResult) {
      return false;
    }

    Serial.print(F(RESPONSE_FLAG));
    Serial.println(F(RESPONSE_OK));
    Serial.flush();
  }
  
  return true;
}
//------------------------------------------------------------------------------

void DisplayBlockError(uint32_t address, uint8_t expected, uint8_t actual)
{
  Serial.flush();
  Serial.print(F(RESPONSE_FLAG));
  Serial.print(F(RESPONSE_ERROR));
  Serial.print(F("Wrote 0x"));
  Serial.print(expected, HEX);
  Serial.print(F(", read 0x"));
  Serial.print(actual, HEX);
  Serial.print(F(", address 0x"));
  Serial.println(address, HEX);
  Serial.flush();
}
//------------------------------------------------------------------------------
// 2716 & 27C16 Eproms
bool WriteBlockX16(uint32_t i)
{
  extern uint8_t EpromVppPin;
  extern uint8_t *SerialBuffer;

  SetDataMode(true);
  
  for(uint8_t j = 0; j < SERIAL_BUF_LEN; j++)
  {
    if(SerialBuffer[j] == 0xFF) {
      continue;
    }
    
    uint8_t registerOne, registerTwo, registerThree;
        
    GenerateAddress(i + j, &registerOne, &registerTwo, &registerThree);
    SetAddress(registerOne, registerTwo, registerThree);
    
    SetVppOn(EpromVppPin);

    WriteDataByte(SerialBuffer[j]);
      
    digitalWrite(NCE_PIN, HIGH);
    delay(45); // should wait between 45 and 55 ms
    digitalWrite(NCE_PIN, LOW);
        
    SetVppOff();

    // Verify byte
    uint8_t verify = VerifyData();
    if(SerialBuffer[j] != verify) 
    {
      DisplayBlockError(i + j, SerialBuffer[j], verify);
      return false;
    }
  }

  return true;
}
//------------------------------------------------------------------------------
// All > 2716 & 27C16 Eproms
bool WriteStandardBlock(uint32_t i)
{
  extern uint8_t EpromVppPin;
  extern uint8_t *SerialBuffer;

  SetDataMode(true);
  
  for(uint8_t j = 0; j < SERIAL_BUF_LEN; j++)
  {
    if(SerialBuffer[j] == 0xFF) {
      continue;
    }
    
    uint8_t registerOne, registerTwo, registerThree;
        
    GenerateAddress(i + j, &registerOne, &registerTwo, &registerThree);
    SetAddress(registerOne, registerTwo, registerThree);

    for(uint8_t k = 1; k <= 10; k++) 
    {
      SetVppOn(EpromVppPin);
  
      WriteDataByte(SerialBuffer[j]);
      
      digitalWrite(NCE_PIN, LOW);
      delay(50); // should wait between 45 and 55 ms
      digitalWrite(NCE_PIN, HIGH);
        
      SetVppOff();
    
      // Verify byte
      uint8_t verify = VerifyData();
      if(SerialBuffer[j] == verify) {
        break;
      }
      else if(k < 10) 
      {
        delay(1);
        continue;
      }

      DisplayBlockError(i + j, SerialBuffer[j], verify);
      return false;
    }
  }

  return true;
}
//------------------------------------------------------------------------------
// 110uS programming impulse
// similar with Flasrite algorithm
bool WriteFastBlock(uint32_t i)
{
  extern uint8_t EpromVppPin;
  extern uint8_t *SerialBuffer;

  SetDataMode(true);  
  for(uint8_t j = 0; j < SERIAL_BUF_LEN; j++)
  {
    if(SerialBuffer[j] == 0xFF) {
      continue;
    }
    
    uint8_t registerOne, registerTwo, registerThree;

    GenerateAddress(i + j, &registerOne, &registerTwo, &registerThree);
    SetAddress(registerOne, registerTwo, registerThree);
    
    for(uint8_t k = 1; k <= 10; k++)
    {
      SetVppOn(EpromVppPin);

      WriteDataByte(SerialBuffer[j]);

      digitalWrite(NCE_PIN, LOW);
      delayMicroseconds(110);
      digitalWrite(NCE_PIN, HIGH);
      
      SetVppOff();
      
      // Verify byte
      uint8_t verify = VerifyData();
      
      if(SerialBuffer[j] == verify) {
        break;
      }
      else if(k < 10) 
      {
        delayMicroseconds(250);
        continue;
      }

      DisplayBlockError(i + j, SerialBuffer[j], verify);
      return false;
    }
  }

  return true;
}
//------------------------------------------------------------------------------

uint8_t VerifyData(void)
{
  StartReadData();
  uint8_t verify = ReadDataByte();
  EndVerifyData();
  return verify;
}
//------------------------------------------------------------------------------

void EndVerifyData(void)
{
  extern EPROM_TYPE EpromSelected;

  SetDataMode(true);
  
  switch(EpromSelected)
  {
    case EPROM_TYPE::X16:
    case EPROM_TYPE::C16:
        digitalWrite(NOE_PIN, HIGH);
        break;

    default:
      digitalWrite(NCE_PIN, HIGH);
      digitalWrite(NOE_PIN, HIGH);
  }
}
//------------------------------------------------------------------------------
