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
//----------------------------------------------------------------------
#include <cstddef>
#include "structs.h"
//----------------------------------------------------------------------

EPROM EpromsList[] =
{
	//------------- NONE -------------
	{
		EPROM_TYPE::NONE, "NONE", "No Eprom", 0, 0, {}, {}, {}
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 2716 -------------
	{
		EPROM_TYPE::X16, "2716 - Generic", "NMOS, 2KiB, Generic", 0x07FF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X16, "2716 - AMD", "NMOS, 2KiB, AMD", 0x07FF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X16, "ET2716", "NMOS, 2KiB, Thomson", 0x07FF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X16, "HN462716", "NMOS, 2KiB, Hitachi", 0x07FF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X16, "2716 - Intel", "NMOS, 2KiB, Intel", 0x07FF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X16, "M2716", "NMOS, 2KiB, SGS-Thomson", 0x07FF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X16, "M5L2716K", "NMOS, 2KiB, Mitsubishi", 0x07FF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X16, "MBM2716", "NMOS, 2KiB, Fujitsu", 0x07FF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X16, "MCM2716", "NMOS, 2KiB, Motorola", 0x07FF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X16, "uPD2716", "NMOS, 2KiB, NEC", 0x07FF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 27C16 -------------
	{
		EPROM_TYPE::C16, "27C16 - Generic", "CMOS, 2KiB, Generic", 0x07FF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::C16, "27C16 - National", "CMOS, 2KiB, National", 0x07FF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::C16, "AM2716B", "CMOS, 2KiB, AMD", 0x07FF + 1, 5.0, { PRG_ALGO::STANDARD }, { 12.5 }, { 5.0 }
	},
    {
		EPROM_TYPE::C16, "NMC27C16B", "CMOS, 2KiB, Fairchild", 0x07FF + 1, 5.0, { PRG_ALGO::STANDARD }, { 12.75 }, { 5.0 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 2732 -------------
	{
		EPROM_TYPE::X32, "2732 - Generic", "NMOS, 4KiB, Generic", 0x0FFF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X32, "AM2732", "NMOS, 4KiB, AMD", 0x0FFF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X32, "AM9732", "NMOS, 4KiB, AMD", 0x0FFF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X32, "F2732", "NMOS, 4KiB, Fairchild", 0x0FFF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X32, "HN462732", "NMOS, 4KiB, Hitachi", 0x0FFF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X32, "2732A", "HMOS, 4KiB, Intel", 0x0FFF + 1, 5.0, { PRG_ALGO::STANDARD }, { 21.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X32, "M2732A", "NMOS, 4KiB, SGS-Thomson", 0x0FFF + 1, 5.0, { PRG_ALGO::STANDARD }, { 21.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X32, "MBM2732", "NMOS, 4KiB, Fujitsu", 0x0FFF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X32, "NTE2732A", "NMOS, 4KiB, NTE", 0x0FFF + 1, 5.0, { PRG_ALGO::STANDARD }, { 21.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X32, "TMM2732D", "NMOS, 4KiB, Toshiba", 0x0FFF + 1, 5.0, { PRG_ALGO::STANDARD }, { 25.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X32, "uPD2732A", "NMOS, 4KiB, NEC", 0x0FFF + 1, 5.0, { PRG_ALGO::STANDARD }, { 21.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 27C32 -------------
	{
		EPROM_TYPE::C32, "27C32 - Generic", "CMOS, 4KiB, Generic", 0x0FFF + 1, 5.0, { PRG_ALGO::STANDARD }, { 12.5 }, { 5.0 }
	},
    {
		EPROM_TYPE::C32, "AM2732B", "CMOS, 4KiB, AMD", 0x0FFF + 1, 5.0, { PRG_ALGO::STANDARD }, { 12.5 }, { 5.0 }
	},
    {
		EPROM_TYPE::C32, "NMC27C32B", "CMOS, 4KiB, Fairchild", 0x0FFF + 1, 5.0, { PRG_ALGO::STANDARD }, { 12.75 }, { 5.0 }
	},
    {
		EPROM_TYPE::C32, "WS57C43C", "CMOS, 4KiB, WSI", 0x0FFF + 1, 5.0, { PRG_ALGO::STANDARD }, { 12.75 }, { 5.0 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 2764 -------------
	{
		EPROM_TYPE::X64, "2764 - Generic", "NMOS, 8KiB, Generic", 0x1FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0 , 5.0 }
	},
    {
		EPROM_TYPE::X64, "2764A - Intel", "HMOS, 8KiB, Intel", 0x1FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X64, "M2764A", "NMOS, 8KiB, ST", 0x1FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.0, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X64, "M5L2764K", "NMOS, 8KiB, Mitsubishi", 0x1FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 21.0, 21.0 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X64, "NTE2764", "NMOS, 8KiB, NTE", 0x1FFF + 1, 5.0, { PRG_ALGO::STANDARD }, { 21.0 }, { 5.0 }
	},
    {
		EPROM_TYPE::X64, "TMM2764AD", "NMOS, 8KiB, Toshiba", 0x1FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 27C64 -------------
	{
		EPROM_TYPE::C64, "27C64 - Generic", "CMOS, 8KiB, Generic", 0x1FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.75 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C64, "27C64 - Microchip", "CMOS, 8KiB, Microchip", 0x1FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C64, "Cy7C266", "CMOS, 8KiB, Cypress", 0x1FFF + 1, 5.0, { PRG_ALGO::STANDARD }, { 12.5 }, { 5.0 }
	},
    {
		EPROM_TYPE::C64, "MBM27C64", "CMOS, 8KiB, Fujitsu", 0x1FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 21.0, 21.0 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C64, "NMC27C64", "CMOS, 8KiB, National", 0x1FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C64, "TMS27C64", "CMOS, 8KiB, TI", 0x1FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 27128 -------------
	{
		EPROM_TYPE::X128, "27128 - Generic", "NMOS, 16KiB, Generic", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::X128, "HN4827128G", "NMOS, 16KiB, Hitachi", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 21.0, 21.0 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C128, "HN27128", "NMOS, 16KiB, Hitachi", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X128, "M27128A - Intel", "HMOS, 16KiB, Intel", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X128, "M27128A - ST", "NMOS, 16KiB, SGS-Thomson", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.0, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X128, "M5L27128K", "NMOS, 16KiB, Mitsubishi", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 21.0, 21.0 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X128, "M5M27C128K", "NMOS, 16KiB, Mitsubishi", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 21.0, 21.0 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X128, "MBM27128", "NMOS, 16KiB, Fujitsu", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 21.0, 21.0 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X128, "NTE21128", "NMOS, 16KiB, NTE", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.0, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X128, "TMM27128AD", "NMOS, 16KiB, Toshiba", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X128, "TMS27128", "NMOS, 16KiB, TI", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 21.0, 21.0 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X128, "uPD27128", "NMOS, 16KiB, NEC", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 21.0, 21.0 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 27C128 -------------
	{
		EPROM_TYPE::C128, "27C128 - Generic", "CMOS, 16KiB, Generic", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.75 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C128, "27C128 - Microchip", "CMOS, 16KiB, Microchip", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C128, "AM27C128", "CMOS, 16KiB, AMD", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C128, "AT27C128", "CMOS, 16KiB, Atmel", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C128, "CY27C128", "CMOS, 16KiB, Cypress", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C128, "LH57126", "CMOS, 16KiB, Sharp", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C128, "M27C128", "CHMOS, 16KiB, Intel", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C128, "MSM27C128", "CMOS, 16KiB, OKI", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 21.0, 21.0 }, { 5.0, 6.0 } 
	},
    {
		EPROM_TYPE::C128, "NM27C128 - Fairchild", "CMOS, 16KiB, Fairchild", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.5 } 
	},
    {
		EPROM_TYPE::C128, "NM27C128 - National", "CMOS, 16KiB, National", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 } 
	},
    {
		EPROM_TYPE::C128, "QP27C128", "CMOS, 16KiB, QPSEMI", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 } 
	},
    {
		EPROM_TYPE::C128, "SMJ27C128", "HVCMOS, 16KiB, TI", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 13.0 }, { 5.0, 6.5 } 
	},
    {
		EPROM_TYPE::C128, "TMS27C128", "CMOS, 16KiB, TI", 0x3FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 27256 -------------
	{
		EPROM_TYPE::X256, "27256 - Generic", "NMOS, 32KiB, Generic", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::X256, "A68C256", "CHMOS, 32KiB, Intel", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::X256, "AT27256", "NMOS, 32KiB, Atmel", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X256, "27256 - Intel", "NMOS, 32KiB, Intel", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::X256, "M5L27256K", "NMOS, 32KiB, Mitsubishi", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X256, "MBM27256", "NMOS, 32KiB, Fujitsu", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X256, "MSM27256", "NMOS, 32KiB, OKI", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::X256, "TMM27256", "NMOS, 32KiB, Toshiba", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X256, "uPD27256", "NMOS, 32KiB, NEC", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 21.0, 21.0 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 27C256 -------------
	{
		EPROM_TYPE::C256, "27C256 - Generic", "CMOS, 32KiB, Generic", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 12.75 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C256, "27C256 - Microchip", "CMOS, 32KiB, Microchip", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C256, "AM27C256", "CMOS, 32KiB, AMD", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C256, "AM27H256", "CMOS, 32KiB, AMD", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C256, "CXK27C256DQ", "CMOS, 32KiB, Sony", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C256, "CY27C256", "CMOS, 32KiB, Cypress", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C256, "CY27H256", "CMOS, 32KiB, Cypress", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C256, "CY7C274", "CMOS, 32KiB, Cypress", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C256, "EM27C256", "CMOS, 32KiB, EM", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C256, "FM27C256", "CMOS, 32KiB, Fairchild", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C256, "HN27C256A", "CMOS, 32KiB, Hitachi", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C256, "HN27C256AG", "CMOS, 32KiB, Hitachi", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C256, "IS27C256", "CMOS, 32KiB, ISSI", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C256, "M27C256B", "CMOS, 32KiB, ST", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C256, "M5M27C256K", "CMOS, 32KiB, Mitsubishi", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C256, "MBM27C256", "CMOS, 32KiB, Fujitsu", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 21.0, 21.0 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C256, "27HC256", "CMOS, 32KiB, Microchip", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C256, "MSM27C256H", "CMOS, 32KiB, OKI", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C256, "NM27C256 - Fairchild", "CMOS, 32KiB, Fairchild", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C256, "NM27C256 - National", "CMOS, 32KiB, National", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C256, "NM27LC256", "CMOS, 32KiB, National", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C256, "NMC27C256", "CMOS, 32KiB, National", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C256, "NMC87C257", "CMOS, 32KiB, National", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C256, "NPX27C256", "CMOS, 32KiB, Philips", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C256, "QP27C256", "CMOS, 32KiB, QPSEMI", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C256, "27C256 - SEEQ", "CMOS, 32KiB, SEEQ", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C256, "M27C256 - SEEQ", "CMOS, 32KiB, SEEQ", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C256, "SMJ27C256 - ASI", "CMOS, 32KiB, ASI", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C256, "SMJ27C256 - TI", "CMOS, 32KiB, TI", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C256, "SPM27C256", "CMOS, 32KiB, Seiko", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C256, "TC57256", "CMOS, 32KiB, Toshiba", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C256, "TMS27C256", "CMOS, 32KiB, TI", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C256, "TMS87C257", "CMOS, 32KiB, TI", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C256, "uPD27C256", "CMOS, 32KiB, NEC", 0x7FFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 21.0, 21.0 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 27512 -------------
	{
		EPROM_TYPE::X512, "27512 - Generic", "NMOS, 64KiB, Generic", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::X512, "27512 - Intel", "HMOS, 64KiB, Intel", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X512, "AM27512", "NMOS, 64KiB, AMD", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 13.0 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X512, "HN27512", "NMOS, 64KiB, Hitachi", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X512, "M27512", "NMOS, 64KiB, SGS", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X512, "M5L27512K", "NMOS, 64KiB, Mitsubishi", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::X512, "MSM27512", "NMOS, 64KiB, OKI", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::X512, "TMM27512AD", "NMOS, 64KiB, Toshiba", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 27C512 -------------
	{
		EPROM_TYPE::C512, "27C512 - Generic", "CMOS, 64KiB, Generic", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 12.75 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C512, "27C512 - Microchip", "CMOS, 64KiB, Microchip", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C512, "27C512 - Intel", "CMOS, 64KiB, Intel", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C512, "27C512 - Philips", "CMOS, 64KiB, Philips", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C512, "AM27C512", "CMOS, 64KiB, AMD", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C512, "CXK27C512DQ", "CMOS, 64KiB, Sony", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C512, "CY27C512", "CMOS, 64KiB, Cypress", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C512, "CY27H512", "CMOS, 64KiB, Cypress", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C512, "EM27C512", "CMOS, 64KiB, EM", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C512, "FM27C512", "CMOS, 64KiB, Fairchild", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C512, "HN27C512", "CMOS, 64KiB, Hitachi", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C512, "IS27HC512", "CMOS, 64KiB, ISSI", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C512, "MBM27C512", "CMOS, 64KiB, Fujitsu", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C512, "MX27C512", "CMOS, 64KiB, Macronix", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C512, "NM27C512", "CMOS, 64KiB, National", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C512, "NM27P512", "CMOS, 64KiB, National", 0xFFFF + 1, 5.0,  { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C512, "SMJ27C512 - ASI", "CMOS, 64KiB, ASI", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C512, "SMJ27C512 - TI", "CMOS, 64KiB, TI", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C512, "TC57512AD", "CMOS, 64KiB, Toshiba", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C512, "TMS27C512", "CMOS, 64KiB, TI", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C512, "uPD27C512", "CMOS, 64KiB, NEC", 0xFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 27C010 -------------
	{
		EPROM_TYPE::C010, "27C010 - Generic", "CMOS, 128KiB, Generic", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C010, "AM27C010", "CMOS, 128KiB, AMD", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C010, "AM27H010", "CMOS, 128KiB, AMD", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C010, "AM27HB010", "CMOS, 128KiB, AMD", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C010, "CY27C010", "CMOS, 128KiB, Cypress", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C010, "CY27H010", "CMOS, 128KiB, Cypress", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C010, "27C010 - Intel", "CMOS, 128KiB, Intel", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C010, "EM27C010", "CMOS, 128KiB, EM", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C010, "FM27C010", "CMOS, 128KiB, Fairchild", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C010, "HN27C101", "CMOS, 128KiB, Hitachi", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C010, "IS27C010", "CMOS, 128KiB, ISSI", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C010, "M5M27C101K", "CMOS, 128KiB, Mitsubishi", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C010, "NM27C010 - Fairchild", "CMOS, 128KiB, Fairchild", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C010, "NM27C010 - National", "CMOS, 128KiB, National", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C010, "NMC27C010", "CMOS, 128KiB, Fairchild", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C010, "SMJ27C010 - ASI", "CMOS, 128KiB, ASI", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C010, "SMJ27C010 - TI", "CMOS, 128KiB, TI", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C010, "TMS27C010", "CMOS, 128KiB, TI", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 27C1001 -------------
	{
		EPROM_TYPE::C1D1, "27C1001 - Generic", "CMOS, 128KiB, Generic", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C1D1, "CXK27C1001DQ", "CMOS, 128KiB, Sony", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C1D1, "M27C1001", "CMOS, 128KiB, ST", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C1D1, "MBM27C1001", "CMOS, 128KiB, Fujitsu", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C1D1, "MSM27C1000", "NMOS, 128KiB, OKI", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C1D1, "MX27C1000", "CMOS, 128KiB, Macronix", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C1D1, "TC57C1001", "CMOS, 128KiB, Toshiba", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C1D1, "TC57H1001", "CMOS, 128KiB, Toshiba", 0x1FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 27C020 -------------
	{
		EPROM_TYPE::C020, "27C020 - Generic", "CMOS, 256KiB, Generic", 0x3FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C020, "AM27C020", "CMOS, 256KiB, AMD", 0x3FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C020, "CY27C020", "CMOS, 256KiB, Cypress", 0x3FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST },  { 12.5, 12.5 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C020, "27C020 - Intel", "CMOS, 256KiB, Intel", 0x3FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C020, "EM27C020", "CMOS, 256KiB, EM", 0x3FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C020, "IS27C020", "CMOS, 256KiB, ISSI", 0x3FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C020, "NM27C020", "CMOS, 256KiB, National", 0x3FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C020, "TMS27C020", "CMOS, 256KiB, TI", 0x3FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 27C2001 -------------
	{
		EPROM_TYPE::C2D1, "27C2001 - Generic", "CMOS, 256KiB, Generic", 0x3FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C2D1, "M27C2001", "CMOS, 256KiB, ST", 0x3FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C2D1, "MSM27C2000", "CMOS, 256KiB, OKI", 0x3FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C2D1, "uPD27C2001", "CMOS, 256KiB, NEC", 0x3FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 27C040 -------------
	{
		EPROM_TYPE::C040, "27C040 - Generic", "CMOS, 512KiB, Generic", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.00, 12.75 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C040, "AM27C040", "CMOS, 512KiB, AMD", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C040, "CY27C040", "CMOS, 512KiB, Cypress", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C040, "EM27C040", "CMOS, 512KiB, EM", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C040, "FM27C040", "CMOS, 512KiB, Fairchild", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C040, "27C040 - Intel", "CMOS, 512KiB, Intel", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C040, "NM27C040", "CMOS, 512KiB, Fairchild", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C040, "NXP27C040", "CMOS, 512KiB, Philips", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C040, "SMJ27C040 - ASI", "CMOS, 512KiB, ASI", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C040, "SMJ27C040 - Micross", "CMOS, 512KiB, Micross", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C040, "SMJ27C040 - TI", "CMOS, 512KiB, TI", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C040, "TMS27C040", "CMOS, 512KiB, TI", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 27C4001 -------------
	{
		EPROM_TYPE::C4D1, "27C4001 - Generic", "CMOS, 512KiB, Generic", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C4D1, "HN27C4001", "CMOS, 512KiB, Hitachi", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.5 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C4D1, "M27C4001", "CMOS, 512KiB, ST", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C4D1, "MBM27C4001", "CMOS, 512KiB, Fujitsu", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.0 }
	},
    {
		EPROM_TYPE::C4D1, "MSM27C401", "CMOS, 512KiB, OKI", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C4D1, "MX27C4000", "CMOS, 512KiB, Macronix", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C4D1, "TC57C4000", "CMOS, 512KiB, Toshiba", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C4D1, "uPD27C4001", "CMOS, 512KiB, NEC", 0x7FFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.5, 12.5 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	},
    //------------- 27C080 -------------
	{
		EPROM_TYPE::C080, "27C080 - Generic", "CMOS, 1MiB, Generic", 0xFFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.00, 12.75 }, { 5.0, 5.0 }
	},
    {
		EPROM_TYPE::C080, "AM27C080", "CMOS, 1MiB, AMD", 0xFFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C080, "AT27LV080", "CMOS, 1MiB, Atmel", 0xFFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 13.0, 13.0 }, { 5.0, 6.5 }
	},
    {
		EPROM_TYPE::C080, "M27C801", "CMOS, 1MiB, ST", 0xFFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::C080, "MX27C8000", "CMOS, 1MiB, Macronix", 0xFFFFF + 1, 5.0, { PRG_ALGO::STANDARD, PRG_ALGO::FAST }, { 12.75, 12.75 }, { 5.0, 6.25 }
	},
    {
		EPROM_TYPE::LAST, "", "", 0, 0, {}, {}, {}
	}
};
size_t EpromsListSize = sizeof(EpromsList) / sizeof(EPROM);
//----------------------------------------------------------------------
