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
//------------------------------------------------------------------------------

void WriteDataByte(uint8_t data);
void StartWriteData(void);
void EndWriteData(void);
uint8_t VerifyData(void);
void EndVerifyData(void);
bool WriteEprom(void);
void DisplayBlockError(uint32_t address, uint8_t expected, uint8_t actual);
bool WriteBlockX16(uint32_t i);
bool WriteStandardBlock(uint32_t i);
bool WriteFastBlock(uint32_t i);
//------------------------------------------------------------------------------
