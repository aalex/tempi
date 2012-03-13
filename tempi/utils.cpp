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
std::string to_string(T value) throw(BadAtomTypeException)
{
    try
    {
        return boost::lexical_cast<std::string>(value);
    }
    catch (const boost::bad_lexical_cast &e)
    {
        throw BadAtomTypeException(e.what());
    }
}

void appendArgumentFromString(Message &message, const char *atom_value, AtomType type)
    throw(BadAtomTypeException)
{
    try
    {
        switch (type)
        {
            case BOOLEAN:
                if (stringsMatch(atom_value, "1"))
                    message.appendBoolean(true);
                else if (stringsMatch(atom_value, "true"))
                    message.appendBoolean(true);
                else if (stringsMatch(atom_value, "0"))
                    message.appendBoolean(false);
                else if (stringsMatch(atom_value, "false"))
                    message.appendBoolean(false);
                else
                    message.appendBoolean(false);
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
            case UNSIGNED_INT:
                message.appendUnsignedInt(
                    boost::lexical_cast<unsigned int>(atom_value));
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
                    throw BadAtomTypeException(os.str().c_str());
                }
                break;
        } // switch typetag
    }
    catch (const boost::bad_lexical_cast &e)
    {
        std::ostringstream os;
        os << "ERROR: " << __FUNCTION__ << 
            ": Bad value \"" << atom_value << "\" to unserialize for type tag \"" << type << "\". " << e.what();
        throw BadAtomTypeException(os.str().c_str());
    }
}

std::string argumentToString(const Message &message, unsigned int index)
    throw(BadAtomTypeException, BadIndexException)
{
    if (index >= message.getSize())
    {
        std::ostringstream os;
        os << __FUNCTION__ << ": Invalid index " << index << " in message " << message;
        throw BadIndexException(os.str().c_str());
    }
    AtomType atom_type;
    message.getAtomType(index, atom_type);
    switch (atom_type)
    {
        case BOOLEAN:
            if (message.getBoolean(index))
                return std::string("true");
            else
                return std::string("false");
            //return boost::lexical_cast<std::string>(
            //    message.getBoolean(index));
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
        case UNSIGNED_INT:
            return boost::lexical_cast<std::string>(
                message.getUnsignedInt(index));
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
                throw BadAtomTypeException(os.str().c_str());
            }
            break;
    } // end of switch/case
}

std::string charToString(const char c)
{
    return boost::lexical_cast<std::string>(c);
}

bool isValidAtomType(const char c)
{
    switch (c)
    {
        case BOOLEAN:
        case CHAR:
        case UNSIGNED_CHAR:
        case UNSIGNED_INT:
        case BANG:
        case DOUBLE:
        case FLOAT:
        case INT:
        case LONG:
        case STRING:
        case POINTER:
            return true;
            break;
        case INVALID:
        default:
            return false;
            break;
    }
}

bool isValidType(const char *type)
{
    std::string typeTag(type);
    unsigned int length = typeTag.size();
    for (unsigned int i = 0; i < length; ++i)
    {
        if (! isValidAtomType(typeTag[i]))
        {
            std::ostringstream os;
            os << __FUNCTION__ << ": Unsupported atom type \"" <<
                typeTag[i] << "\" at " << i << " in " << type << std::endl;
            return false;
        }
    }
    return true;
}

Message castMessage(const Message &message, const char *type)
    throw(BadAtomTypeException, BadIndexException)
{
    if (! isValidType(type))
    {
        std::ostringstream os;
        os << __FUNCTION__ << ": Bad type tag: " << type;
        throw BadAtomTypeException(os.str().c_str());
    }
    std::string desiredTypeTag(type);
    if (desiredTypeTag.size() != message.getSize())
    {
        std::ostringstream os;
        os << __FUNCTION__ << ": Type tag does not match the message's size: " << type << message;
        throw BadIndexException(os.str().c_str());
    }
    unsigned int size = message.getSize();
    Message result;
    for (unsigned int i = 0; i < size; ++i)
    {
        AtomType current;
        message.getAtomType(i, current);
        AtomType desired = (AtomType) desiredTypeTag[i];

        // CONVERSION
        bool success = true;
        switch (desired)
        {
            case FLOAT: // desired is FLOAT
                switch (current)
                {
                    case FLOAT:
                        result.appendFloat(message.getFloat(i));
                        break;
                    case BOOLEAN:
                        result.appendFloat((float) message.getBoolean(i));
                        break;
                    case CHAR:
                        result.appendFloat((float) message.getChar(i));
                        break;
                    case UNSIGNED_CHAR:
                        result.appendFloat((float) message.getUnsignedChar(i));
                        break;
                    case UNSIGNED_INT:
                        result.appendFloat((float) message.getUnsignedInt(i));
                        break;
                    case BANG:
                        result.appendFloat(0.0f);
                        break;
                    case DOUBLE:
                        result.appendFloat((float) message.getDouble(i));
                        break;
                    case INT:
                        result.appendFloat((float) message.getInt(i));
                        break;
                    case LONG:
                        result.appendFloat((float) message.getLong(i));
                        break;
                    case STRING:
                        try
                        {
                            appendArgumentFromString(result,
                                message.getString(i).c_str(), FLOAT);
                        }
                        catch(const BadAtomTypeException &e)
                        {
                            std::cerr << e.what() << std::endl;
                            success = false;
                        }
                        break;
                    case POINTER:
                        result.appendFloat((float) (long) message.getPointer(i));
                        break;
                    case INVALID:
                    default:
                        success = false;
                        break;
                } // switch desired
                break; // case FLOAT
            case STRING: // desired is STRING
                try
                {
                    result.appendString(argumentToString(message, i).c_str());
                }
                catch (const BadIndexException &e)
                {
                    std::cerr << e.what() << std::endl;
                    success = false;
                }
                catch (const BadAtomTypeException &e)
                {
                    std::cerr << e.what() << std::endl;
                    success = false;
                }
                break; // case STRING
            case UNSIGNED_CHAR: // desired is UNSIGNED_CHAR
                switch (current)
                {
                    case FLOAT:
                        result.appendUnsignedChar((unsigned char) message.getFloat(i));
                        break;
                    case BOOLEAN:
                        result.appendUnsignedChar((unsigned char) message.getBoolean(i));
                        break;
                    case CHAR:
                        result.appendUnsignedChar((unsigned char) message.getChar(i));
                        break;
                    case UNSIGNED_CHAR:
                        result.appendUnsignedChar(message.getUnsignedChar(i));
                        break;
                    case UNSIGNED_INT:
                        result.appendUnsignedChar((unsigned char) message.getUnsignedInt(i));
                        break;
                    case BANG:
                        result.appendUnsignedChar(' ');
                        break;
                    case DOUBLE:
                        result.appendUnsignedChar((unsigned char) message.getDouble(i));
                        break;
                    case INT:
                        result.appendUnsignedChar((unsigned char) message.getInt(i));
                        break;
                    case LONG:
                        result.appendUnsignedChar((unsigned char) message.getLong(i));
                        break;
                    case STRING:
                        try
                        {
                            appendArgumentFromString(result,
                                message.getString(i).c_str(), UNSIGNED_CHAR);
                        }
                        catch(const BadAtomTypeException &e)
                        {
                            std::cerr << e.what() << std::endl;
                            success = false;
                        }
                        break;
                    case POINTER:
                        result.appendUnsignedChar((unsigned char) (long) message.getPointer(i));
                        break;
                    case INVALID:
                    default:
                        success = false;
                        break;
                } // switch desired
                break; // case UNSIGNED_CHAR
            // TODO: support casting to BOOLEAN
            // TODO: support casting to CHAR
            // TODO: support casting to DOUBLE
            // TODO: support casting to INT
            // TODO: support casting to LONG
            default:
                success = false;
            // boost::lexical_cast<char>(atom_value));
            //     result.appendFloat((float) message.getInt(i));
            if (! success)
            {
                std::ostringstream os;
                os << "ERROR: " << __FUNCTION__ << 
                    ": Could not cast message to \"" << desired << "\": " << message;
                throw BadAtomTypeException(os.str().c_str());
            }
        } // switch current
    }
    return result;
}

} // end of namespace
} // end of namespace

