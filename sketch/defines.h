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
#pragma once

#include <Arduino.h>
//------------------------------------------------------------------------------

#define OPTIMIZE_FOR_SPEED

// 74HC595
#define SER_PIN A0
#define SCK_PIN A1
#define RCK_PIN A2

// Data
#define D0_PIN 7
#define D1_PIN 8
#define D2_PIN 9
#define D3_PIN 6
#define D4_PIN 5
#define D5_PIN 4
#define D6_PIN 3
#define D7_PIN 2

// Eprom control
#define NCE_PIN     A3
#define NOE_PIN     A4
#define A13_VCC_PIN A5
#define A17_VCC_PIN 13
#define NO_VCC_PIN  0
#define MUX_B0_PIN  12
#define MUX_B1_PIN  11
#define MUX_B2_PIN  10
#define A11_VPP_PIN 1
#define NOE_VPP_PIN 3
#define A15_VPP_PIN 5
#define A19_VPP_PIN 7
#define NO_VPP_PIN  0

// Voltage test
#define VPP_TEST_PIN    A7
#define VPP_R_PWR_VALUE 9944.0
#define VPP_R_GND_VALUE 894.0
#define VCC_TEST_PIN    A6
#define VCC_R_PWR_VALUE 9974.0
#define VCC_R_GND_VALUE 4662.0
#define AREF_VALUE      2.466

#define SERIAL_BUF_LEN 32
#define TEST_STEP_WAIT 2500
#define VOLTAGE_CHECKS 250

#define MIN_VCC 4.50
#define MAX_VCC 6.50
#define MIN_VPP 10.00
#define MAX_VPP 30.00

// commands
#define COMMAND_FLAG           "@CMD"
#define COMMAND_SET_EPROM_TYPE "TYPE"
#define COMMAND_VOLTAGE_INFO   "VINF"
#define COMMAND_READ_EPROM     "READ"
#define COMMAND_WRITE_EPROM    "WRIT"
#define COMMAND_TEST           "TEST"
#define RESPONSE_FLAG          "@RSP"
#define RESPONSE_OK            "OK  "
#define RESPONSE_ERROR         "ERR "
#define RESPONSE_BLOCK         "BLCK"
#define RESPONSE_READ_EPROM    COMMAND_READ_EPROM
#define RESPONSE_VOLTAGE_INFO  COMMAND_VOLTAGE_INFO
#define RESPONSE_TEST          COMMAND_TEST
#define RESPONSE_WRITE_EPROM   COMMAND_WRITE_EPROM
#define COMMAND_TEST_SPEED     "SPED"

enum COMMAND_MODE {
  WAIT,
  READ,
  WRITE,
  VOLTAGE,
  TEST,
  TEST_SPEED
};

enum EPROM_TYPE {
  NONE = 0,
  X16  = 1,  // 2716
  C16  = 2,  // 27C16
  X32  = 3,  // 2732
  C32  = 4,  // 27C32
  X64  = 5,  // 2764
  C64  = 6,  // 27C64
  X128 = 7,  // 27128
  C128 = 8,  // 27C128
  X256 = 9,  // 27256
  C256 = 10, // 27C256
  X512 = 11, // 27512
  C512 = 12, // 27C512
  C010 = 13, // 27C010
  C1D1 = 14, // 27C1001
  C020 = 15, // 27C020
  C2D1 = 16, // 27C2001
  C040 = 17, // 27C040
  C4D1 = 18, // 27C4001
  C080 = 19, // 27C080
  LAST_EPROM_TYPE
};

enum PRG_ALGO {
  UNDEFINED        = 0,
  STANDARD         = 1,
  FAST             = 2,
  LAST_ALGO
};
//------------------------------------------------------------------------------
