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

#include "tempi/message.h"
#include "tempi/exceptions.h"
#include "tempi/log.h"
#include <iostream>
#include <sstream>
#include <ostream>
#include <cstring>
#include <limits> // numeric_limits::epsilon
#include <cmath> // fabs

namespace tempi {

Message::Message()
{
    // pass
}

Message::Message(const char *types, ...)
{
    // if (Logger::isEnabledFor(DEBUG))
    // {
    //     std::ostringstream os;
    //     os << "Message." << __FUNCTION__ << "(" << types << ", ...)";
    //     Logger::log(DEBUG, os);
    // }
    va_list arguments;
    va_start(arguments, types);
    appendVaList(types, arguments);
    va_end(arguments);
}

void Message::append(const char *types, ...)
{
    // if (Logger::isEnabledFor(DEBUG))
    // {
    //     std::ostringstream os;
    //     os << "Message." << __FUNCTION__ << ": " << types;
    //     Logger::log(DEBUG, os);
    // }
    va_list arguments;
    va_start(arguments, types);
    appendVaList(types, arguments);
    va_end(arguments);
}

void Message::appendVaList(const char *types, va_list arguments)
{
    // if (Logger::isEnabledFor(DEBUG))
    // {
    //     std::ostringstream os;
    //     os << "Message." << __FUNCTION__ << ": " << types;
    //     Logger::log(DEBUG, os);
    // }
    // TODO: avoid segfault if length does not match
    // TODO: avoid segfault if types do not match
    for (int i = 0; types[i] != '\0'; ++i)
    {
        switch (types[i])
        {
            case NONE:
            {
                appendNone();
                break;
            }
            case FLOAT:
            {
                Float value = va_arg(arguments, double);
                //std::cout << "Caught a float : " << value;
                appendFloat(value);
                break;
            }
            case INT:
            {
                Int value = va_arg(arguments, Int);
                //std::cout << "Caught an integer : " << value;
                appendInt(value);
                break;
            }
            case BOOLEAN:
            {
                bool value = (bool) va_arg(arguments, int);
                //std::cout << "Caught a bool : " << value;
                appendBoolean(value);
                break;
            }
            case STRING:
            {
                const char *value = va_arg(arguments, const char*);
                // FIXME: does this memleak?
                //std::cout << "Caught a string : " << value;
                appendString(value);
                break;
            }
            case POINTER:
            {
                void *value = va_arg(arguments, void *);
                //std::cout << "Caught a pointer :" << value;
                appendPointer(value);
                break;
            }
            // case BLOB:
            // {
            //     atom::BlobValue::ptr value = va_arg(arguments, atom::BlobValue::ptr);
            //     //std::cout << "Caught a blob :" << value;
            //     appendBlob(value);
            //     break;
            // }
            default:
                {
                    std::ostringstream os;
                    os << "Message::" << __FUNCTION__ << ": Unsupported type tag: " << types[i] << " (types=\"" << types << "\") (length=" << std::strlen(types) << ")";
                    Logger::log(ERROR, os);
                }
                break;
        }
        //std::cout << std::endl;
    }
}

bool Message::append(boost::any value)
{
    arguments_.push_back(value);
    return true;
}

bool Message::prepend(boost::any value)
{
    arguments_.insert(arguments_.begin(), value);
    return true;
}

Index Message::getSize() const
{
    return arguments_.size();
}

const boost::any *Message::getArgument(Index index) const // throw(BadIndexException)
{
    if (index >= getSize())
    {
        // std::ostringstream os;
        // os << "Message::" << __FUNCTION__ << ": Index too big: " << index << std::endl;
        // throw BadIndexException(os.str.c_str());
        return 0;
    }
    else
    {
        return &arguments_[index];
    }
}

void Message::prependBlob(atom::BlobValue::ptr value)
{
    prepend(boost::any(value));
}

void Message::appendBlob(atom::BlobValue::ptr value)
{
    append(boost::any(value));
}

atom::BlobValue::ptr Message::getBlob(Index index) const
{
    atom::BlobValue::ptr value;
    get<atom::BlobValue::ptr>(index, value);
    return value;
}

void Message::prependFloat(Float value)
{
    prepend(boost::any(value));
}

void Message::prependInt(Int value)
{
    prepend(boost::any(value));
}

void Message::prependNone()
{
    prepend(boost::any(None()));
}

void Message::appendNone()
{
    append(boost::any(None()));
}

void Message::prependStdString(std::string value)
{
    prepend(boost::any(value));
}

void Message::prependBoolean(bool value)
{
    prepend(boost::any(value));
}

void Message::prependPointer(void *value)
{
    prepend(boost::any(value));
}

void Message::appendFloat(Float value)
{
    append(boost::any(value));
}

void Message::appendInt(Int value)
{
    append(boost::any(value));
}

void Message::appendStdString(std::string value)
{
    append(boost::any(value));
}

void Message::appendBoolean(bool value)
{
    append(boost::any(value));
}

void Message::appendPointer(void *value)
{
    append(boost::any(value));
}

Int Message::getInt(Index index) const throw(BadAtomTypeException, BadIndexException)
{
    Int value;
    get<Int>(index, value);
    return value;
}

std::string Message::getString(Index index) const throw(BadAtomTypeException, BadIndexException)
{
    std::string value;
    get<std::string>(index, value);
    return value;
}

Float Message::getFloat(Index index) const throw(BadAtomTypeException, BadIndexException)
{
    Float value;
    get<Float>(index, value);
    return value;
}

bool Message::getBoolean(Index index) const throw(BadAtomTypeException, BadIndexException)
{
    bool value;
    get<bool>(index, value);
    return value;
}

void *Message::getPointer(Index index) const throw(BadAtomTypeException, BadIndexException)
{
    void *value;
    get<void *>(index, value);
    return value;
}

void Message::setBoolean(Index index, bool value) throw(BadAtomTypeException, BadIndexException)
{
    set<bool>(index, value);
}

void Message::setFloat(Index index, Float value) throw(BadAtomTypeException, BadIndexException)
{
    set<Float>(index, value);
}

void Message::setInt(Index index, Int value) throw(BadAtomTypeException, BadIndexException)
{
    set<Int>(index, value);
}

void Message::setString(Index index, std::string value) throw(BadAtomTypeException, BadIndexException)
{
    set<std::string>(index, value);
}

void Message::setPointer(Index index, void *value) throw(BadAtomTypeException, BadIndexException)
{
    set<void *>(index, value);
}

namespace types {

bool getAtomTypeForAny(const boost::any &value, AtomType &type)
{
    const std::type_info &actual = value.type();
    if (actual == typeid(void))
        return false;
    if (actual == typeid(bool))
        type = BOOLEAN;
    else if (actual == typeid(Float))
        type = FLOAT;
    else if (actual == typeid(Int))
        type = INT;
    else if (actual == typeid(std::string))
        type = STRING;
    else if (actual == typeid(None))
        type = NONE;
    else if (actual == typeid(void *))
        type = POINTER;
    else if (actual == typeid(atom::BlobValue::ptr))
        type = BLOB;
    else
    {
        std::cerr << __FUNCTION__ << ": Could not figure out type of value. It's " << actual.name() << std::endl;
        std::cerr << __FUNCTION__ << ": By comparison, BLOB type id is " << typeid(atom::BlobValue::ptr).name() << std::endl;
        return false;
    }
    return true;
}

} // end of namespace

bool Message::getAtomType(Index index, AtomType &type) const
{
    const boost::any *tmp = getArgument(index);
    if (tmp)
        return types::getAtomTypeForAny(*tmp, type);
    else
        return false;
}

bool Message::typesMatch(std::string &types) const
{
    if (types.size() != getSize())
        return false;
    Index index = 0;
    AtomType actual;
    std::string::iterator iter;
    for (iter = types.begin(); iter < types.end(); ++iter)
    {
        getAtomType(index, actual);
        if ((*iter) != actual)
            return false;
        ++index;
    }
    return true;
}

bool Message::indexMatchesType(Index index, char type) const
{
    AtomType actual;
    if ( ! getAtomType(index, actual))
        return false;
    return actual == type;
}

std::string Message::getTypes() const
{
    std::string ret;
    for (Index i = 0; i < getSize(); ++i)
    {
        AtomType type;
        if (! getAtomType(i, type))
            std::cerr << "Message::" << __FUNCTION__ << ": Could not get type for arg" << i << std::endl;
        else
            ret.append(1, (char) type);
    }
    return ret;
}

bool Message::operator!=(const Message &other) const
{
    return ! (*this == other);
}

static bool doublesMatch(double left, double right)
{
    return std::fabs(left - right) > std::numeric_limits<double>::epsilon();
}

bool Message::operator==(const Message &other) const
{
    if (getTypes() != other.getTypes())
    {
        return false;
    }
    for (Index i = 1; i < getSize(); ++i)
    {
        AtomType type;
        getAtomType(i, type);
        switch (type)
        {
            case BOOLEAN:
                if (getBoolean(i) != other.getBoolean(i))
                    return false;
                break;
            case NONE:
                // always matches
                break;
            case FLOAT:
                if (! doublesMatch(getFloat(i), other.getFloat(i)))
                    return false;
                break;
            case INT:
                if (getInt(i) != other.getInt(i))
                    return false;
                break;
            case STRING:
                if (getString(i) != other.getString(i))
                    return false;
                break;
            case POINTER:
                if (getPointer(i) != other.getPointer(i))
                    return false;
                break;
            case BLOB:
                {
                    atom::BlobValue::ptr left = getBlob(i);
                    atom::BlobValue::ptr right = other.getBlob(i);
                    if (left != right)
                        return false;
                    break;
                }
            defaut:
                std::cerr << "Message" << __FUNCTION__ << ": Unsupported type." << std::endl;
                break;
        }
    }
    return true;
}

std::ostream &operator<<(std::ostream &os, const Message &message)
{
    os << "(";
    for (int i = 0; i < (int) message.getSize(); ++i)
    {
        AtomType type;
        message.getAtomType(i, type);
        os << (char) type << ":";
        switch (type)
        {
            case BOOLEAN:
                os << (message.getBoolean(i) ? "true" : "false");
                break;
            case FLOAT:
                os << message.getFloat(i);
                break;
            case INT:
                os << message.getInt(i);
                break;
            case STRING:
                os << "\"" << message.getString(i) << "\"";
                break;
            case BLOB:
                {
                    // Used to print pointer address:
                    // const char * blob_value = message.getBlob(i)->getValue();
                    // os << &blob_value;
                    os << message.getBlob(i)->getHex();
                    break;
                }
            defaut:
                std::cerr << "std::ostream &operator<<(std::ostream &os, const Message &message): Unsupported type." << std::endl;
                break;
        }
        if (i < (message.getSize() - 1))
            os << ", ";
    }
    os << ")";
    return os;
}

void Message::prependMessage(const Message &message)
{
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "Message." << __FUNCTION__ << ": " << message << " to " << (*this);
        Logger::log(DEBUG, os);
    }
    if (message.getSize() == 0)
    {
        //std::cout << __FUNCTION__ << " empty message. nothing to append\n";
        return;
    }
    for (Index i = message.getSize() - 1; i >= 0; --i)
    {
        AtomType type;
        //std::cout << __FUNCTION__ << " " << i << std::endl;
        try
        {
            bool ok = message.getAtomType(i, type);
            if (! ok)
            {
                std::cerr << "Error trying to access atom out of bound: " << i
                    << " in " << message;
                break;
            }
        }
        catch (const BadIndexException &e) // FIXME: getAtomType doesn't throw any exception!
        {
            std::cerr << __FUNCTION__ << " " << e.what() << std::endl;
            //throw (e);
        }
        switch (type)
        {
            case BOOLEAN:
                prependBoolean(message.getBoolean(i));
                break;
            case FLOAT:
                prependFloat(message.getFloat(i));
                break;
            case INT:
                prependInt(message.getInt(i));
                break;
            case NONE:
                prependNone();
                break;
            case STRING:
                prependString(message.getString(i).c_str());
                break;
            case BLOB:
                prependBlob(message.getBlob(i));
                break;
            default:
                std::cerr << "Message::" << __FUNCTION__ << "(): Unknow type of atom: " << type << std::endl;
                break;
        }
        if (i == 0)
            break; // FIXME
    }
}

} // end of namespace

