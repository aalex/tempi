#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright (C) 2011 Alexandre Quessy
# 
# This file is part of Tempi.
# 
# Tempi is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# Tempi is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Tempi.  If not, see <http://www.gnu.org/licenses/>.

header = """
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
"""

footer = """

} // end of namespace

} // end of namespace

#endif // ifndef
"""

def get_one(prefix, suffix):
    types = {
        "i": "int",
        "f": "float",
        "d": "double",
        "c": "char",
        "b": "bool",
        "s": "std::string",
    }
    return prefix + 

def print_types():
    for first in types.iterkeys():
        for depth in range(10):
            for _type in types.iterkeys():
                print("typedef boost::tuple<")


if __name__ == "__main__":
    print(header)
    print_types()
    print(footer)
