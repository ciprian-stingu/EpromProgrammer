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
#include "read.h"
#include "io.h"
//------------------------------------------------------------------------------

//read a byte from eprom
uint8_t ReadDataByte(void)
{
#ifndef OPTIMIZE_FOR_SPEED
  uint8_t data = 0;
  
  data |= digitalRead(D0_PIN) << 0;
  data |= digitalRead(D1_PIN) << 1;
  data |= digitalRead(D2_PIN) << 2;
  data |= digitalRead(D3_PIN) << 3;
  data |= digitalRead(D4_PIN) << 4;
  data |= digitalRead(D5_PIN) << 5;
  data |= digitalRead(D6_PIN) << 6;
  data |= digitalRead(D7_PIN) << 7;

  return data;
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
  
  uint8_t resultB = *portInputRegister(portB) & maskB;
  uint8_t resultD = *portInputRegister(portD) & maskD;

  uint8_t data = 0;
  if(resultD & maskD0) {
    data |= 0x01;
  }
  if(resultB & maskD1) {
    data |= 0x02;
  }
  if(resultB & maskD2) {
    data |= 0x04;
  }
  if(resultD & maskD3) {
    data |= 0x08;
  }
  if(resultD & maskD4) {
    data |= 0x10;
  }
  if(resultD & maskD5) {
    data |= 0x20;
  }
  if(resultD & maskD6) {
    data |= 0x40;
  }
  if(resultD & maskD7) {
    data |= 0x80;
  }
  return data;
#endif
}
//------------------------------------------------------------------------------

void StartReadData(void)
{
  SetDataMode(false);
  
  digitalWrite(NCE_PIN, LOW);
  digitalWrite(NOE_PIN, LOW);
}
//------------------------------------------------------------------------------

void EndReadData(void)
{
  digitalWrite(NCE_PIN, HIGH);
  digitalWrite(NOE_PIN, HIGH);
}
//------------------------------------------------------------------------------

void ReadEprom(void)
{
  extern uint32_t EpromEndAddress;
  extern uint8_t *SerialBuffer;
  
  StartReadData();

  delay(10);
  
  for(uint32_t i = 0; i < EpromEndAddress; i += SERIAL_BUF_LEN)
  {
    for(uint8_t j = 0; j < SERIAL_BUF_LEN; j++) 
    {
      uint8_t registerOne, registerTwo, registerThree;
      
      GenerateAddress(i + j, &registerOne, &registerTwo, &registerThree);
      SetAddress(registerOne, registerTwo, registerThree);
      
      SerialBuffer[j] = ReadDataByte();
    }

    Serial.write(SerialBuffer, SERIAL_BUF_LEN);
    Serial.flush();
  }
  
  EndReadData();
}
//------------------------------------------------------------------------------
