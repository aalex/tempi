/*
 * Copyright (C) 2011 Alexandre Quessy
 * 
 * This file is part of Tempi.
 * 
 * Tempi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Tempi.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 * Functions in the tempi::utils namespace.
 */
#ifndef __TEMPI_UTILS_H__
#define __TEMPI_UTILS_H__

#include <string>

namespace tempi
{

namespace utils
{

bool stringBeginsWith(const char *text, const char *pattern);
bool stringsMatch(const char *a, const char *b);
int map_int(int value, int istart, int istop, int ostart, int ostop);
float map_float(float value, float istart, float istop, float ostart, float ostop);
std::string concatenate(const char *a, const char *b);

} // end of namespace

} // end of namespace

#endif // ifndef

