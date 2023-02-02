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
#include "test.h"
//------------------------------------------------------------------------------
// global variables
uint8_t SerialBufferStorage[SERIAL_BUF_LEN + 1];
uint8_t *SerialBuffer = SerialBufferStorage;
COMMAND_MODE CommandMode = COMMAND_MODE::WAIT;
EPROM_TYPE EpromSelected = EPROM_TYPE::NONE;
PRG_ALGO ProgrammingAlgorithm = PRG_ALGO::STANDARD;
double VppRequired = 0.0;
double VccRequired = 0.0;
uint32_t EpromEndAddress = 0; 
uint8_t EpromVppPin = 0;
//------------------------------------------------------------------------------

int8_t StringPos(uint8_t *bytes, const char *test, uint8_t offset = 0);
//------------------------------------------------------------------------------

void setup() 
{
  // set voltage reference to external
  analogReference(EXTERNAL);
  analogRead(VPP_TEST_PIN);
  analogRead(VCC_TEST_PIN);
    
  // 74HC595
  pinMode(SER_PIN, OUTPUT);
  pinMode(SCK_PIN, OUTPUT);
  pinMode(RCK_PIN, OUTPUT);

  pinMode(NCE_PIN, OUTPUT);
  pinMode(NOE_PIN, OUTPUT);
  pinMode(A13_VCC_PIN, OUTPUT);
  pinMode(A17_VCC_PIN, OUTPUT);

  // set mux output
  pinMode(MUX_B0_PIN, OUTPUT);
  pinMode(MUX_B1_PIN, OUTPUT);
  pinMode(MUX_B2_PIN, OUTPUT);
  
  InitializeEpromToAKnownState();

  SerialBuffer[SERIAL_BUF_LEN] = 0x00;
  
  Serial.begin(115200);
  Serial.println(F("27[C]XXXX programmer"));
  Serial.flush();

  CommandMode = COMMAND_MODE::WAIT;
}
//------------------------------------------------------------------------------

void loop() 
{
  switch(CommandMode)
  {
    case COMMAND_MODE::READ:
      if(EpromSelected == EPROM_TYPE::NONE)
      {
        Serial.print(F(RESPONSE_FLAG));
        Serial.print(F(RESPONSE_ERROR));
        Serial.println(F("No EPROM selected!"));
        Serial.flush();
        CommandMode = COMMAND_MODE::WAIT;
        break;
      }
      
      Serial.print(F(RESPONSE_FLAG));
      Serial.println(F(RESPONSE_READ_EPROM));
      Serial.flush();
      
      ReadEprom();

      Serial.print(F(RESPONSE_FLAG));
      Serial.println(F(RESPONSE_OK));
      Serial.flush();
      
      CommandMode = COMMAND_MODE::WAIT;
      break;
      
    case COMMAND_MODE::WRITE:
      if(EpromSelected == EPROM_TYPE::NONE)
      {
        Serial.print(F(RESPONSE_FLAG));
        Serial.print(F(RESPONSE_ERROR));
        Serial.println(F("No EPROM selected!"));
        Serial.flush();
        CommandMode = COMMAND_MODE::WAIT;
        break;
      }
      else if(VccRequired < MIN_VCC || VccRequired > MAX_VCC || VppRequired < MIN_VPP || VppRequired > MAX_VPP) 
      {
        Serial.print(F(RESPONSE_FLAG));
        Serial.print(F(RESPONSE_ERROR));
        Serial.println(F("Invalid VCC/VPP range specified!"));
        Serial.flush();
        CommandMode = COMMAND_MODE::WAIT;
        break;
      }
      else
      {
        double vccVoltage = GetVccVoltage();
        double vppVoltage = GetVppVoltage();
        if(vccVoltage < VccRequired - 0.25 || vccVoltage > VccRequired + 0.25)
        {
          Serial.print(F(RESPONSE_FLAG));
          Serial.print(F(RESPONSE_ERROR));
          Serial.print(F("Vcc: "));
          Serial.print(vccVoltage, 2);
          Serial.print(F("V != "));
          Serial.print(VccRequired, 2);
          Serial.println(F("V"));
          Serial.flush();
          CommandMode = COMMAND_MODE::WAIT;
          break;
        }
        if(vppVoltage < VppRequired - 0.25 || vppVoltage > VppRequired + 0.25)
        {
          Serial.print(F(RESPONSE_FLAG));
          Serial.print(F(RESPONSE_ERROR));
          Serial.print(F("Vpp: "));
          Serial.print(vppVoltage, 2);
          Serial.print(F("V != "));
          Serial.print(VppRequired, 2);
          Serial.println(F("V"));
          Serial.flush();
          CommandMode = COMMAND_MODE::WAIT;
          break;
        }
      }

      Serial.print(F(RESPONSE_FLAG));
      Serial.println(F(RESPONSE_WRITE_EPROM));
      Serial.flush();
      
      {
        if(ProgrammingAlgorithm == PRG_ALGO::STANDARD) {
           Serial.setTimeout(750);
        }
        else {
          Serial.setTimeout(400);
        }
        bool writeResult = WriteEprom();
        Serial.setTimeout(1000);
        EndWriteData();
        
        if(writeResult)
        {
          Serial.print(F(RESPONSE_FLAG));
          Serial.println(F(RESPONSE_WRITE_EPROM));
          Serial.flush();
        }
      }
      
      CommandMode = COMMAND_MODE::WAIT;
      break;
      
    case COMMAND_MODE::VOLTAGE:
      {
        double vccVoltage = GetVccVoltage();
        double vppVoltage = GetVppVoltage();

        Serial.print(F(RESPONSE_FLAG));
        Serial.print(F(RESPONSE_VOLTAGE_INFO));
        Serial.print(F("[VCC] "));
        Serial.print(vccVoltage, 2);
        Serial.print(F("|[VPP] "));
        Serial.println(vppVoltage, 2);
        Serial.flush();
      }
      CommandMode = COMMAND_MODE::WAIT;
      break;
      
    case COMMAND_MODE::TEST: //just to be sure
      if(EpromSelected != EPROM_TYPE::NONE)
      {
        Serial.print(F(RESPONSE_FLAG));
        Serial.print(F(RESPONSE_ERROR));
        Serial.println(F("EPROM is selected!"));
        Serial.flush();
        CommandMode = COMMAND_MODE::WAIT;
        break;
      }
      
      Serial.print(F(RESPONSE_FLAG));
      Serial.print(F(RESPONSE_TEST));
      Serial.println(F(" ON"));
      TestMode();
      Serial.print(F(RESPONSE_FLAG));
      Serial.print(F(RESPONSE_TEST));
      Serial.println(F(" OFF"));
      Serial.flush();
      CommandMode = COMMAND_MODE::WAIT;
      break;

    case COMMAND_MODE::TEST_SPEED: //just to be sure
      if(EpromSelected != EPROM_TYPE::NONE)
      {
        Serial.print(F(RESPONSE_FLAG));
        Serial.print(F(RESPONSE_ERROR));
        Serial.println(F("EPROM is selected!"));
        Serial.flush();
        CommandMode = COMMAND_MODE::WAIT;
        break;
      }
      
      Serial.print(F(RESPONSE_FLAG));
      Serial.print(F(RESPONSE_TEST));
      Serial.println(F(" SPEED ON"));
      TestSpeed();
      Serial.print(F(RESPONSE_FLAG));
      Serial.print(F(RESPONSE_TEST));
      Serial.println(F(" SPEED OFF"));
      Serial.flush();
      CommandMode = COMMAND_MODE::WAIT;
      break;
      
    default:
      if(!Serial.available()) {
        break;
      }

      for(uint8_t i = 0; i < SERIAL_BUF_LEN + 1; i++) {
        SerialBuffer[i] = 0x00;
      }
      
      uint8_t count = Serial.readBytes(SerialBuffer, SERIAL_BUF_LEN);
    
      while(Serial.available()) {
        Serial.read();
      }

      if(count)
      {
        bool commandFound = true;
        int8_t commandFlagIndex = StringPos(SerialBuffer, COMMAND_FLAG);
        
        if(commandFlagIndex != -1)
        {          
          int8_t commandTypeIndex = -1;
          
          if((commandTypeIndex = StringPos(SerialBuffer, COMMAND_SET_EPROM_TYPE, commandFlagIndex + 4)) != -1) 
          {
            String command((char*)SerialBuffer);
            String tmp = command.substring(commandTypeIndex + 4, commandTypeIndex + 6);
            int epromType = tmp.toInt();
            if(epromType >= EPROM_TYPE::NONE && epromType < EPROM_TYPE::LAST_EPROM_TYPE) {
              SelectEprom((EPROM_TYPE)epromType);
            }
            else
            {
              EpromSelected = EPROM_TYPE::NONE;
              commandFound = false;
            }
          }
          else if((commandTypeIndex = StringPos(SerialBuffer, COMMAND_READ_EPROM, commandFlagIndex + 4)) != -1) {
            CommandMode = COMMAND_MODE::READ;
          }
          else if((commandTypeIndex = StringPos(SerialBuffer, COMMAND_WRITE_EPROM, commandFlagIndex + 4)) != -1)
          {
            String command((char*)SerialBuffer);
            String tmp = command.substring(commandTypeIndex + 4, commandTypeIndex + 9);
            VccRequired = tmp.toFloat();
            if(VccRequired >= MIN_VCC && VccRequired <= MAX_VCC)
            {
              tmp = command.substring(commandTypeIndex + 9, commandTypeIndex + 14);
              VppRequired = tmp.toFloat();
              if(VppRequired >= MIN_VPP && VppRequired <= MAX_VPP)
              {
                tmp = command.substring(commandTypeIndex + 14, commandTypeIndex + 16);
                int testProgrammingAlgorithm = tmp.toInt();
                if(testProgrammingAlgorithm >= (int)PRG_ALGO::STANDARD && testProgrammingAlgorithm < (int)PRG_ALGO::LAST_ALGO) 
                {
                  CommandMode = COMMAND_MODE::WRITE;
                  ProgrammingAlgorithm = (PRG_ALGO)testProgrammingAlgorithm;
                }
                else 
                {
                  ProgrammingAlgorithm = PRG_ALGO::STANDARD;
                  VppRequired = 0;
                  VccRequired = 0;
                  commandFound = false;
                }
              }
              else
              {
                ProgrammingAlgorithm = PRG_ALGO::STANDARD;
                VppRequired = 0;
                VccRequired = 0;
                commandFound = false;
              }
            }
            else
            {
              ProgrammingAlgorithm = PRG_ALGO::STANDARD;
              VppRequired = 0;
              VccRequired = 0;
              commandFound = false;
            }
          }
          else if(StringPos(SerialBuffer, COMMAND_VOLTAGE_INFO, commandFlagIndex + 4) != -1) {
            CommandMode = COMMAND_MODE::VOLTAGE;
          }
          else if(StringPos(SerialBuffer, COMMAND_TEST, commandFlagIndex + 4) != -1) {
            CommandMode = COMMAND_MODE::TEST;
          }
          else if(StringPos(SerialBuffer, COMMAND_TEST_SPEED, commandFlagIndex + 4) != -1) {
            CommandMode = COMMAND_MODE::TEST_SPEED;
          }
          else {
            commandFound = false;
          }
        }
        else {
          commandFound = false;
        }

        Serial.print(F(RESPONSE_FLAG));
        if(commandFound) {
          Serial.println(F(RESPONSE_OK));
        }
        else 
        {
          CommandMode = COMMAND_MODE::WAIT;
          Serial.print(F(RESPONSE_ERROR));
          Serial.print(F("["));
          Serial.write(SerialBuffer, SERIAL_BUF_LEN);
          Serial.println(F("]"));
        }
        Serial.flush();
      }
  }
}
//------------------------------------------------------------------------------

int8_t StringPos(uint8_t *bytes, const char *test, uint8_t offset)
{
  for(int8_t i = offset, j = SERIAL_BUF_LEN - 4; i < j; i++)
  {
    uint8_t k = 0;
    for(; k < 4; k++)
    {
      if(bytes[i] != (uint8_t)test[k]) {
        break;    
      }
      i++;
    }
    if(k == 4) {
      return i - k;
    }
  }
  return -1;
}
//------------------------------------------------------------------------------
