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

#ifndef __TEMPI_TYPES_H__
#define __TEMPI_TYPES_H__

#include <string>
#include <boost/tuple/tuple.hpp>

namespace tempi
{

namespace types
{

typedef double f;
typedef boost::tuple<double, double> ff;
typedef boost::tuple<double, double, double> fff;
typedef boost::tuple<double, double, double, double> ffff;

typedef int i;
typedef boost::tuple<int, int> ii;
typedef boost::tuple<int, int, int> iii;
typedef boost::tuple<int, int, int, int> iiii;

typedef std::string s;
typedef boost::tuple<std::string, std::string> ss;
typedef boost::tuple<std::string, std::string, std::string> sss;
typedef boost::tuple<std::string, std::string, std::string, std::string> ssss;

} // end of namespace

} // end of namespace

#endif // ifndef

