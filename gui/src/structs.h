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
#ifndef STRUCTS_H
#define STRUCTS_H
//------------------------------------------------------------------------------
#include <stdint.h>
#include "defines.h"
//------------------------------------------------------------------------------

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
    LAST
};

enum PRG_ALGO {
    UNDEFINED        = 0,
    STANDARD         = 1,
    FAST             = 2
};

const char *const PRG_ALGO_NAMES[] =
{
    "None selected",
    "Standard",
    "Fast",
};

typedef struct EPROM {
    EPROM_TYPE  EpromType;
    const char  *EpromName;
    const char  *EpromDescription;
    int         MemorySize;
    double      VccVoltage;
    PRG_ALGO    ProgrammingAlgorithm[PROGRAMMING_ALGORITHMS];
    double      VppVoltage[PROGRAMMING_ALGORITHMS];
    double      ProgrammingVccVoltage[PROGRAMMING_ALGORITHMS];
} EPROM;

//----------------------------------------------------------------------                
#endif

