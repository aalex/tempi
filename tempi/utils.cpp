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

#include "tempi/utils.h"
#include <algorithm> // min, max
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <string.h>

namespace tempi {
namespace utils {

bool stringBeginsWith(const char *text, const char *pattern)
{
    size_t length = strlen(pattern);
    if (strlen(text) < length)
        return false;
    if (strncmp(text, pattern, length))
        return true;
}

bool stringsMatch(const char *a, const char *b)
{
    if (! a && ! b)
        return true;
    else if (! a)
        return false;
    else if (! b)
        return false;
    else
        return strcmp(a, b) == 0;
    // FIXME: would be safer to use strncmp
}

/**
 * Convenience function to map a variable from one coordinate space
 * to another.
 * The result is clipped in the range [ostart, ostop]
 * Make sure ostop is bigger than ostart.
 *
 * To map a MIDI control value into the [0,1] range:
 * map(value, 0.0, 1.0, 0. 127.);
 *
 * Depends on: #include <algorithm>
 */
float map_float(float value, float istart, float istop, float ostart, float ostop)
{
    float ret = ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
    // In Processing, they don't do the following: (clipping)
    return std::max(std::min(ret, ostop), ostart);
}

int map_int(int value, int istart, int istop, int ostart, int ostop)
{
    float ret = ostart + (ostop - ostart) * ((value - istart) / float(istop - istart));
    //g_print("%f = %d + (%d-%d) * ((%d-%d) / (%d-%d))", ret, ostart, ostop, ostart, value, istart, istop, istart);
    // In Processing, they don't do the following: (clipping)
    return std::max(std::min(int(ret), ostop), ostart);
}

std::string concatenate(const char *a, const char *b)
{
    std::string ret(a);
    ret += b;
    return ret;
}

template <typename T>
std::string to_string(T value) throw(BadArgumentTypeException)
{
    try
    {
        return boost::lexical_cast<std::string>(value);
    }
    catch (const boost::bad_lexical_cast &e)
    {
        throw BadArgumentTypeException(e.what());
    }
}

void appendArgumentFromString(Message &message, const char *atom_value, ArgumentType type)
    throw(BadArgumentTypeException)
{
    try
    {
        switch (type)
        {
            case BOOLEAN:
                if (atom_value == "0")
                    message.appendBoolean(false);
                else
                    message.appendBoolean(true);
                break;
            case CHAR:
                message.appendChar(
                    boost::lexical_cast<char>(atom_value));
                break;
            case UNSIGNED_CHAR:
                message.appendUnsignedChar(
                    boost::lexical_cast<unsigned char>(atom_value));
                break;
            case DOUBLE:
                message.appendDouble(
                    boost::lexical_cast<double>(atom_value));
                break;
            case FLOAT:
                message.appendFloat(
                    boost::lexical_cast<float>(atom_value));
                break;
            case INT:
                message.appendInt(
                    boost::lexical_cast<int>(atom_value));
                break;
            case LONG:
                message.appendLong(
                    boost::lexical_cast<unsigned long long>(atom_value));
                break;
            case STRING:
                message.appendString(atom_value);
                break;
            case POINTER:
            default:
                {
                    std::ostringstream os;
                    os << "ERROR: " << __FUNCTION__ << 
                        ": Unsupported type tag to unserialize: " <<
                        (char) type;
                    throw BadArgumentTypeException(os.str().c_str());
                }
                break;
        } // switch typetag
    }
    catch (const boost::bad_lexical_cast &e)
    {
        std::ostringstream os;
        os << "ERROR: " << __FUNCTION__ << 
            ": Bad value \"" << atom_value << "\" to unserialize for type tag \"" << type << "\". " << e.what();
        throw BadArgumentTypeException(os.str().c_str());
    }
}

std::string argumentToString(const Message &message, unsigned int index)
    throw(BadArgumentTypeException, BadIndexException)
{
    if (index >= message.getSize())
    {
        std::ostringstream os;
        os << __FUNCTION__ << ": Invalid index " << index << " in message " << message;
        throw BadIndexException(os.str().c_str());
    }
    ArgumentType atom_type;
    message.getArgumentType(index, atom_type);
    switch (atom_type)
    {
        case BOOLEAN:
            return boost::lexical_cast<std::string>(
                message.getBoolean(index));
            break;
        case CHAR:
            return boost::lexical_cast<std::string>(
                message.getChar(index));
            break;
        case UNSIGNED_CHAR:
            return boost::lexical_cast<std::string>(
                message.getUnsignedChar(index));
            break;
        case DOUBLE:
            return boost::lexical_cast<std::string>(
                message.getDouble(index));
            break;
        case FLOAT:
            return boost::lexical_cast<std::string>(
                message.getFloat(index));
            break;
        case INT:
            return boost::lexical_cast<std::string>(
                message.getInt(index));
            break;
        case LONG:
            return boost::lexical_cast<std::string>(
                message.getLong(index));
            break;
        case STRING:
            return message.getString(index);
            break;
        case POINTER:
            return boost::lexical_cast<std::string>(
                message.getPointer(index));
            break;
        default:
            {
                std::ostringstream os;
                os << __FUNCTION__ << ": Unsupported atom type \"" << atom_type << "\" to serialize at index " << index << " " << atom_type;
                throw BadArgumentTypeException(os.str().c_str());
            }
            break;
    } // end of switch/case
}

} // end of namespace
} // end of namespace

