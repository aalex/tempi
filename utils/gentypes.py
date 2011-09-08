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

HEADER = """ /*
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
#include <boost/any.hpp>
#include <boost/tuple/tuple.hpp>

namespace tempi
{

namespace types
{

using std::string;
using boost::any;
using boost::tuple;

"""

FOOTER = """

} // end of namespace

} // end of namespace

#endif // ifndef
"""

TYPES = {
    "a": "any",
    "b": "bool",
    # "c": "char",
    # "d": "double",
    "f": "float", # or double?
    "i": "int",
    "s": "string",
    # "u": "unsigned int",
}

MAX_LENGTH = 3

def print_types():
    lines = []
    positions = []
    # for pos in range(MAX_LENGTH):
    #    positions.append(0)
    for length_minus_one in range(MAX_LENGTH):
        word_length = length_minus_one + 1
        number_of_words_of_this_length = len(TYPES) ** word_length
        for permutation_number in range(number_of_words_of_this_length):
            prefix = "typedef tuple<"
            name = ""
            for char_pos in range(word_length):
                index = (permutation_number + char_pos) % len(TYPES)
                # print "XXXXXXXXXXXXXXXXXXXXXXXXX", index
                name += TYPES.keys()[index]
                prefix += TYPES.values()[index]
                if char_pos < (word_length - 1):
                    prefix += ", "
            prefix += ">"
            line = "{:<50}".format(prefix) + " " + name + ";"
            lines.append(line)
    for line in lines:
        print(line)

if __name__ == "__main__":
    print(HEADER)
    print_types()
    print(FOOTER)

