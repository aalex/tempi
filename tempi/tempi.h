/*
 * Copyright (C) 2011 Alexandre Quessy
 *
 * This file is part of Tempi.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software ither version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tempi.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 * Single public header file for tempi.
 *
 * Includes the other header files.
 */

#ifndef __TEMPI_H__
#define __TEMPI_H__

#include <string>

/**
 * @namespace tempi
 *
 * C++ library that deals with timed events.
 *
 * The whole library is within this single namespace.
 */

namespace tempi
{

/**
 * Prints a greeting.
 */
void hello();

/**
 * Returns the version of Tempi as a string.
 */
std::string get_version();

}; // end of namespace

#endif // ifndef

