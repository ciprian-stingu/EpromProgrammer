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
#ifndef DEFINES_H
#define DEFINES_H
//------------------------------------------------------------------------------
//os detection
#if defined(__linux__)
	#define OS_IS_LINUX 1
#elif defined(_WIN32) || defined(_WIN64)
	#define OS_IS_WINDOWS 1
#endif

#define MAKE_STR(x) _MAKE_STR(x)
#define _MAKE_STR(x) #x

#define APP_PACKAGE "EpGui"
#define APP_VERSION "1.0"

#define DEFAULT_LOG_FILENAME APP_PACKAGE ".log"
#define CONFIG_DIRNAME "." APP_PACKAGE
#define CONFIG_FILENAME APP_PACKAGE ".ini"

#define PROGRAMMER_NAME "27[C]XXXX programmer"
#define LOG_MAX_LINES_DISPLAYED 1000 
#define DISPLAY_COLUMNS 16
#define BLOCK_SIZE 32
#define PROGRAMMING_ALGORITHMS 2

//#define ALT_EPROMS

#if defined(OS_IS_LINUX)
  #define ATTR_UNUSED __attribute__((unused))
#elif defined(OS_IS_WINDOWS)
  #define ATTR_UNUSED
#endif
//------------------------------------------------------------------------------
#endif
