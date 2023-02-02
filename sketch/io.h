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

void SetDataMode(bool write);
void SetAddress(uint8_t registerOne, uint8_t registerTwo, uint8_t registerThree);
void SetVppOff(void);
void SetVppOn(uint8_t vppPin);
void InitializeEpromToAKnownState(void);
void SelectEprom(EPROM_TYPE newEprom);
void GenerateAddress(uint32_t address, uint8_t *registerOne, uint8_t *registerTwo, uint8_t *registerThree);
double GetVppVoltage(void);
double GetVccVoltage(void);
void WaitForData(void);
//------------------------------------------------------------------------------
