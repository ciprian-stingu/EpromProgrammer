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
//------------------------------------------------------------------------------
// set read/write eprom mode
void SetDataMode(bool write)
{
#ifndef OPTIMIZE_FOR_SPEED
  pinMode(D0_PIN, write ? OUTPUT : INPUT_PULLUP);
  pinMode(D1_PIN, write ? OUTPUT : INPUT_PULLUP);
  pinMode(D2_PIN, write ? OUTPUT : INPUT_PULLUP);
  pinMode(D3_PIN, write ? OUTPUT : INPUT_PULLUP);
  pinMode(D4_PIN, write ? OUTPUT : INPUT_PULLUP);
  pinMode(D5_PIN, write ? OUTPUT : INPUT_PULLUP);
  pinMode(D6_PIN, write ? OUTPUT : INPUT_PULLUP);
  pinMode(D7_PIN, write ? OUTPUT : INPUT_PULLUP);
#else
  uint8_t portB = digitalPinToPort(D1_PIN);
  uint8_t portD = digitalPinToPort(D0_PIN);

  uint8_t maskD0 = digitalPinToBitMask(D0_PIN);
  uint8_t maskD1 = digitalPinToBitMask(D1_PIN);
  uint8_t maskD2 = digitalPinToBitMask(D2_PIN);
  uint8_t maskD3 = digitalPinToBitMask(D3_PIN);
  uint8_t maskD4 = digitalPinToBitMask(D4_PIN);
  uint8_t maskD5 = digitalPinToBitMask(D5_PIN);
  uint8_t maskD6 = digitalPinToBitMask(D6_PIN);
  uint8_t maskD7 = digitalPinToBitMask(D7_PIN);

  uint8_t maskB = maskD1 | maskD2;
  uint8_t maskD = maskD0 | maskD3 | maskD4 | maskD5 | maskD6 | maskD7;
    
  if(write)
  {
    volatile uint8_t *regB = portModeRegister(portB);
    volatile uint8_t *regD = portModeRegister(portD);

    uint8_t backupSREG = SREG;
    cli();
    
    *regB |= maskB;
    *regD |= maskD;
    
    SREG = backupSREG;
  }
  else
  {
    volatile uint8_t *regB = portModeRegister(portB);
    volatile uint8_t *regD = portModeRegister(portD);
    volatile uint8_t *outB = portOutputRegister(portB);
    volatile uint8_t *outD = portOutputRegister(portD);

    uint8_t backupSREG = SREG;
    cli();

    *regB &= ~maskB;
    *outB |= maskB;
    *regD &= ~maskD;
    *outD |= maskD;
    
    SREG = backupSREG;
  }
#endif
}
//------------------------------------------------------------------------------

void GenerateAddress(uint32_t address, uint8_t *registerOne, uint8_t *registerTwo, uint8_t *registerThree)
{
  extern EPROM_TYPE EpromSelected;
  extern uint32_t EpromEndAddress;
  extern COMMAND_MODE CommandMode;
  
  *registerOne = 0;
  *registerTwo = 0;
  *registerThree = 0;
  
  switch(EpromSelected)
  {
    case EPROM_TYPE::X16:
    case EPROM_TYPE::C16:
      address &= EpromEndAddress;
      *registerOne = address & 0xFF;
      *registerTwo = (address & 0xFF00) >> 8;
      // A11 is VPP which in case of read need to be at VCC
      if(CommandMode == READ) {
        *registerTwo |= 0x08;
      }
      break;
      
    case EPROM_TYPE::X64:
    case EPROM_TYPE::C64:
    case EPROM_TYPE::X128:
    case EPROM_TYPE::C128:
      address &= EpromEndAddress;
      *registerOne = address & 0xFF;
      *registerTwo = (address & 0xFF00) >> 8;
      // A14 is nPGM whci in case of read need to be at VCC
      // A15 is VPP which in case of read need to be at VCC
      if(CommandMode == READ) {
        *registerTwo |= 0xC0;
      }
      break;

    case EPROM_TYPE::X256:
    case EPROM_TYPE::C256:
      address &= EpromEndAddress;
      *registerOne = address & 0xFF;
      *registerTwo = (address & 0xFF00) >> 8;
      // A15 is VPP which in case of read need to be at VCC
      if(CommandMode == READ) {
        *registerTwo |= 0x80;
      }
      break;
      
    default:
      address &= EpromEndAddress;
      *registerOne = address & 0xFF;
      *registerTwo = (address & 0xFF00) >> 8;
      *registerThree = (address & 0xFF0000) >> 16;
      break;
  }
}
//------------------------------------------------------------------------------
// set the address using 3 register values
// registerOne   = addresses A0  -> A7
// registerTwo   = addresses A8  -> A15
// registerThree = addresses A16 -> A19 (the most significative 4 bits are 0)
void SetAddress(uint8_t registerOne, uint8_t registerTwo, uint8_t registerThree)
{
  digitalWrite(RCK_PIN, LOW);
#ifndef OPTIMIZE_FOR_SPEED
  shiftOut(SER_PIN, SCK_PIN, MSBFIRST, registerThree);
  shiftOut(SER_PIN, SCK_PIN, MSBFIRST, registerTwo);
  shiftOut(SER_PIN, SCK_PIN, MSBFIRST, registerOne);
#else
  uint8_t bitmaskSER = digitalPinToBitMask(SER_PIN);
  volatile uint8_t *outputSER = portOutputRegister(digitalPinToPort(SER_PIN));
  
  uint8_t bitmaskSCK = digitalPinToBitMask(SCK_PIN);
  volatile uint8_t *outputSCK = portOutputRegister(digitalPinToPort(SCK_PIN));

  //registerThree
  uint8_t backupSREG = SREG;
  cli();
  for(uint8_t i = 0; i < 8; i++)  
  {
    if((registerThree & (1 << (7 - i))) == LOW) {
      *outputSER &= ~bitmaskSER;
    }
    else {
      *outputSER |= bitmaskSER;
    }
    
    *outputSCK |= bitmaskSCK;
    *outputSCK &= ~bitmaskSCK;
  }
  SREG = backupSREG;
  
  //registerTwo
  backupSREG = SREG;
  cli();
  for(uint8_t i = 0; i < 8; i++)  
  {    
    if((registerTwo & (1 << (7 - i))) == LOW) {
      *outputSER &= ~bitmaskSER;
    }
    else {
      *outputSER |= bitmaskSER;
    }

    *outputSCK |= bitmaskSCK;
    *outputSCK &= ~bitmaskSCK;
  }
  SREG = backupSREG;
  
  //registerOne
  backupSREG = SREG;
  cli();
  for(uint8_t i = 0; i < 8; i++)  
  {
    if((registerOne & (1 << (7 - i))) == LOW) {
      *outputSER &= ~bitmaskSER;
    }
    else {
      *outputSER |= bitmaskSER;
    }
    
    *outputSCK |= bitmaskSCK;
    *outputSCK &= ~bitmaskSCK;
  }
  SREG = backupSREG;
#endif
  digitalWrite(RCK_PIN, HIGH);
}
//------------------------------------------------------------------------------
//disable vpp
void SetVppOff(void)
{
  digitalWrite(MUX_B0_PIN, LOW);
  digitalWrite(MUX_B1_PIN, LOW);
  digitalWrite(MUX_B2_PIN, LOW);
}
//------------------------------------------------------------------------------
//enable one pin for vpp
void SetVppOn(uint8_t vppPin)
{
  switch(vppPin)
  {
    case A11_VPP_PIN:
      digitalWrite(MUX_B0_PIN, HIGH);
      break;
      
    case NOE_VPP_PIN:
      digitalWrite(MUX_B1_PIN, HIGH);
      digitalWrite(MUX_B0_PIN, HIGH);
      break;
      
    case A15_VPP_PIN:
      digitalWrite(MUX_B2_PIN, HIGH);
      digitalWrite(MUX_B0_PIN, HIGH);
      break;
      
    case A19_VPP_PIN:
      digitalWrite(MUX_B2_PIN, HIGH);
      digitalWrite(MUX_B1_PIN, HIGH);
      digitalWrite(MUX_B0_PIN, HIGH);
      break;
      
    default:
      SetVppOff();
  }
}
//------------------------------------------------------------------------------
//reset the circuit to a known state
void InitializeEpromToAKnownState(void)
{
  // disable nCE
  digitalWrite(NCE_PIN, HIGH);
  // disable nOE
  digitalWrite(NOE_PIN, HIGH);
  // disable A13 VCC
  digitalWrite(A13_VCC_PIN, LOW);
  // disable A17 VCC
  digitalWrite(A17_VCC_PIN, LOW);

  // disable VPP
  SetVppOff();

  // Data
  SetDataMode(false);

  // no address
  SetAddress(0, 0, 0);
}
//------------------------------------------------------------------------------

void SelectEprom(EPROM_TYPE newEprom)
{
  extern EPROM_TYPE EpromSelected;
  extern uint8_t EpromVppPin;
  extern uint32_t EpromEndAddress;
  
  InitializeEpromToAKnownState();
  EpromSelected = newEprom;
  uint8_t epromVccPin = NO_VCC_PIN;
  
  switch(newEprom)
  {
    case EPROM_TYPE::X16:
    case EPROM_TYPE::C16:
      epromVccPin = A13_VCC_PIN;
      EpromVppPin = A11_VPP_PIN;
      EpromEndAddress = 0x07FF;
      break;
      
    case EPROM_TYPE::X32:
    case EPROM_TYPE::C32:
      epromVccPin = A13_VCC_PIN;
      EpromVppPin = NOE_VPP_PIN;
      EpromEndAddress = 0x0FFF;
      break;
      
    case EPROM_TYPE::X64:
    case EPROM_TYPE::C64:
      epromVccPin = A17_VCC_PIN;
      EpromVppPin = A15_VPP_PIN;
      EpromEndAddress = 0x1FFF;
      break;
      
    case EPROM_TYPE::X128:
    case EPROM_TYPE::C128:
      epromVccPin = A17_VCC_PIN;
      EpromVppPin = A15_VPP_PIN;
      EpromEndAddress = 0x3FFF;
      break;
      
    case EPROM_TYPE::X256:
    case EPROM_TYPE::C256:
      epromVccPin = A17_VCC_PIN;
      EpromVppPin = A15_VPP_PIN;
      EpromEndAddress = 0x7FFF;
      break;
      
    case EPROM_TYPE::X512:
    case EPROM_TYPE::C512:
      epromVccPin = A17_VCC_PIN;
      EpromVppPin = NOE_VPP_PIN;
      EpromEndAddress = 0xFFFF;
      break;

    case EPROM_TYPE::C010:
    case EPROM_TYPE::C1D1:
      epromVccPin = NO_VCC_PIN;
      EpromVppPin = A19_VPP_PIN;
      EpromEndAddress = 0x1FFFF;
      break;

    case EPROM_TYPE::C020:
    case EPROM_TYPE::C2D1:
      epromVccPin = NO_VCC_PIN;
      EpromVppPin = A19_VPP_PIN;
      EpromEndAddress = 0x3FFFF;
      break;
      
    case EPROM_TYPE::C040:
    case EPROM_TYPE::C4D1:
      epromVccPin = NO_VCC_PIN;
      EpromVppPin = A19_VPP_PIN;
      EpromEndAddress = 0x7FFFF;
      break;

    case EPROM_TYPE::C080:
      epromVccPin = NO_VCC_PIN;
      EpromVppPin = NOE_VPP_PIN;
      EpromEndAddress = 0xFFFFF;
      break;

    default:
      epromVccPin = NO_VCC_PIN;
      EpromVppPin = NO_VPP_PIN;
      EpromSelected = NONE;
      EpromEndAddress = 0;
  }

  if(EpromSelected != EPROM_TYPE::NONE && epromVccPin != NO_VCC_PIN) {
    digitalWrite(epromVccPin, HIGH);
  }
}
//------------------------------------------------------------------------------
//get the average vpp voltage
double GetVppVoltage(void)
{
  double vppVoltage = 0.0;
  for(uint8_t i = 0; i < VOLTAGE_CHECKS; i++) 
  {
    vppVoltage += (analogRead(VPP_TEST_PIN) * AREF_VALUE) / 1023.0;
    delay(1);
  }
  vppVoltage /= (VOLTAGE_CHECKS * VPP_R_GND_VALUE);
  return (vppVoltage * (VPP_R_PWR_VALUE + VPP_R_GND_VALUE));
}
//------------------------------------------------------------------------------
//get the average vcc voltage
double GetVccVoltage(void)
{
  double vccVoltage = 0.0;
  for(uint8_t i = 0; i < VOLTAGE_CHECKS; i++) 
  {
    vccVoltage += (analogRead(VCC_TEST_PIN) * AREF_VALUE) / 1023.0;
    delay(1);
  }
  vccVoltage /= (VOLTAGE_CHECKS * VCC_R_GND_VALUE);
  return (vccVoltage * (VCC_R_PWR_VALUE + VCC_R_GND_VALUE));
}
//------------------------------------------------------------------------------

void WaitForData(void)
{
  unsigned long start = millis();
  while(Serial.available() < 0)
  {
    if(millis() - start > 1000) {
      break;
    }
    delay(1);
  }
}
//------------------------------------------------------------------------------
