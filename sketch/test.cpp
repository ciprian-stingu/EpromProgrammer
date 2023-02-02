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
#include "test.h"
#include "io.h"
#include "write.h"
#include "read.h"
//------------------------------------------------------------------------------
//check the hardware
void TestMode(void)
{
  InitializeEpromToAKnownState();
  SetDataMode(true);
  WriteDataByte(0);
  digitalWrite(NCE_PIN, LOW);
  digitalWrite(NOE_PIN, LOW);
  delay(500);

  double vccVoltage = GetVccVoltage();
  double vppVoltage = GetVppVoltage();
  Serial.print(F("* Vcc voltage "));
  Serial.println(vccVoltage, 2);
  Serial.print(F("* Vpp voltage "));
  Serial.println(vppVoltage, 2);
  //Serial.flush();
  
  TestDataPin(D0_PIN, "D0");
  TestDataPin(D1_PIN, "D1");
  TestDataPin(D2_PIN, "D2");
  TestDataPin(D3_PIN, "D3");
  TestDataPin(D4_PIN, "D4");
  TestDataPin(D5_PIN, "D5");
  TestDataPin(D6_PIN, "D6");
  TestDataPin(D7_PIN, "D7");

  TestAddressPin(0b00000001, 0, 0, "A0");
  TestAddressPin(0b00000010, 0, 0, "A1");
  TestAddressPin(0b00000100, 0, 0, "A2");
  TestAddressPin(0b00001000, 0, 0, "A3");
  TestAddressPin(0b00010000, 0, 0, "A4");
  TestAddressPin(0b00100000, 0, 0, "A5");
  TestAddressPin(0b01000000, 0, 0, "A6");
  TestAddressPin(0b10000000, 0, 0, "A7");

  TestAddressPin(0, 0b00000001, 0, "A8");
  TestAddressPin(0, 0b00000010, 0, "A9");
  TestAddressPin(0, 0b00000100, 0, "A10");
  TestAddressPin(0, 0b00001000, 0, "A11");
  TestAddressPin(0, 0b00010000, 0, "A12");
  TestAddressPin(0, 0b00100000, 0, "A13");
  TestAddressPin(0, 0b01000000, 0, "A14");
  TestAddressPin(0, 0b10000000, 0, "A15");

  TestAddressPin(0, 0, 0b00000001, "A16");
  TestAddressPin(0, 0, 0b00000010, "A17");
  TestAddressPin(0, 0, 0b00000100, "A18");
  TestAddressPin(0, 0, 0b00001000, "A19");

  TestFunctionPin(NCE_PIN, "Setting nCE HIGH...", "Setting nCE LOW...");
  TestFunctionPin(NOE_PIN, "Setting nOE HIGH...", "Setting nOE LOW...");
  TestFunctionPin(A13_VCC_PIN, "Setting A13 to VCC...", "Setting A13 to GND...");
  TestFunctionPin(A17_VCC_PIN, "Setting A17 to VCC...", "Setting A17 to GND...");
  
  TestVppPin(NOE_VPP_PIN, "nOE");
  TestVppPin(A11_VPP_PIN, "A11");
  TestVppPin(A15_VPP_PIN, "A15");
  TestVppPin(A19_VPP_PIN, "A19");
  
  InitializeEpromToAKnownState();
}
//------------------------------------------------------------------------------
//check data pins
void TestDataPin(uint8_t pin, const char *pinName)
{
  Serial.print(F("* Setting "));
  Serial.print(pinName);
  Serial.println(F(" HIGH..."));
  //Serial.flush();
  digitalWrite(pin, HIGH);
  delay(TEST_STEP_WAIT);
  Serial.print(F("* Setting "));
  Serial.print(pinName);
  Serial.println(F(" LOW..."));
  //Serial.flush();
  digitalWrite(pin, LOW);
  delay(TEST_STEP_WAIT);
}
//------------------------------------------------------------------------------
//check others functional pins
void TestFunctionPin(uint8_t pin, const char *onMessage, const char *offMessage)
{
  Serial.print(F("* "));
  Serial.println(onMessage);
  //Serial.flush();
  digitalWrite(pin, HIGH);
  delay(TEST_STEP_WAIT);
  Serial.print(F("* "));
  Serial.println(offMessage);
  //Serial.flush();
  digitalWrite(pin, LOW);
  delay(TEST_STEP_WAIT);
}
//------------------------------------------------------------------------------
//check address pins
void TestAddressPin(uint8_t registerOne, uint8_t registerTwo, uint8_t registerThree, const char *pinName)
{
  Serial.print(F("* Setting "));
  Serial.print(pinName);
  Serial.println(F(" HIGH..."));
  //Serial.flush();
  SetAddress(registerOne, registerTwo, registerThree);
  delay(TEST_STEP_WAIT);
  Serial.print(F("* Setting "));
  Serial.print(pinName);
  Serial.println(F(" LOW..."));
  //Serial.flush();
  SetAddress(0, 0, 0);
  delay(TEST_STEP_WAIT);
}
//------------------------------------------------------------------------------
//check vpp pins
void TestVppPin(uint8_t vppPin, const char *pinName)
{
  Serial.print(F("* Setting "));
  Serial.print(pinName);
  Serial.println(F(" to VPP..."));
  //Serial.flush();
  SetVppOn(vppPin);
  delay(TEST_STEP_WAIT);
  Serial.print(F("* Setting "));
  Serial.print(pinName);
  Serial.println(F(" to GND..."));
  //Serial.flush();
  SetVppOff();
  delay(TEST_STEP_WAIT);  
}
//------------------------------------------------------------------------------

void TestSpeed(void)
{
  unsigned long time = micros();
  for(int i = 0; i < 2500; i++) 
  {
//    //7.60 uS
//    digitalWrite(NCE_PIN, LOW);
//    digitalWrite(NCE_PIN, HIGH);

//      //A11_VPP_PIN - 17.78 uS
//      //A19_VPP_PIN - 26.46 uS
//    SetVppOn(A11_VPP_PIN); 
//    SetVppOff();
  
  
//    //776.62 uS
//    //172.85 uS - OPTIMIZED
//    SetAddress(0xFF, 0xFF, 0b00001111);
//    SetAddress(0x00, 0x00, 0x00);
  
  
//    //119.96 uS
//    //87.39 uS - OPTIMIZED
//    //27.03 uS
//    SetDataMode(true);
//    WriteDataByte(0x00);
//    SetDataMode(false);

    //77.40 uS
    //17.03 uS - OPTIMIZED
    SetDataMode(true);
    SetDataMode(false);

//    //141.47 uS
//    //102.92 uS - OPTIMIZED
//    //41.93 uS
//    VerifyData();

//    //121.09 uS
//    //85.57 uS - OPTIMIZED
//    //25.20 uS
//    SetDataMode(true);
//    uint8_t a = ReadDataByte();
//    SetDataMode(false);
    
  }
  time = micros() - time;
  float rate = (float)(time) / (float)(2500);
  Serial.print(F("Rate: "));
  Serial.println(rate, 2);
}
//------------------------------------------------------------------------------
