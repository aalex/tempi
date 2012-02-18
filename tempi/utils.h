/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 *
 * This file is part of Tempi.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of, either version 3 of the License, or
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
#include <vector>
#include <algorithm>
#include "tempi/exceptions.h"
#include "tempi/message.h"

namespace tempi {
namespace utils {

/**
 * Checks if a strings begins with another one.
 */
bool stringBeginsWith(const char *text, const char *pattern);

/**
 * Checks if a strings matches another one.
 */
bool stringsMatch(const char *a, const char *b);

/**
 * Maps an int value to a new range.
 */
int map_int(int value, int istart, int istop, int ostart, int ostop);

/**
 * Maps a float value to a new range.
 */
float map_float(float value, float istart, float istop, float ostart, float ostop);

/**
 * Concatenates two strings together.
 */
std::string concatenate(const char *a, const char *b);

/**
 * Converts any type (int, etc.) to a string.
 */
template <typename T>
std::string to_string(T value) throw(BadArgumentTypeException);

/**
 * Checks if a value is in a vector.
 */
template <typename T>
bool find_in_vector(std::vector<T> &vec, const T &value)
{
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}

/**
 * Serializes a given argument from a Message.
 */
std::string argumentToString(const Message &message, unsigned int index)
    throw(BadArgumentTypeException, BadIndexException);

/**
 * Appends an argument to the given Message parsing a serialized string, if valid.
 */
void appendArgumentFromString(Message &message, const char *atom_value, ArgumentType type)
    throw(BadArgumentTypeException);

/**
 * Simply converts a char to a string.
 */
std::string charToString(const char c);

/**
 * Checks if a type tag contains valid letters or not.
 */
bool isValidType(const char *type);
/**
 * Checks if a character represents a valid type tag.
 */
bool isValidAtomType(const char c);

/**
 * Tries to cast a Message arguments to some other type tags.
 */
Message castMessage(const Message &message, const char *type)
    throw(BadArgumentTypeException, BadIndexException);

} // end of namespace
} // end of namespace

#endif // ifndef

