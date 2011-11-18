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

#include "tempi/message.h"
#include "tempi/exceptions.h"
#include <iostream>
#include <sstream>
#include <ostream>

namespace tempi
{

Message::Message()
{
    // pass
}

Message::Message(const char *types, ...)
{
    va_list arguments;
    va_start(arguments, types);
    appendVaList(types, arguments);
    va_end(arguments);
}

void Message::append(const char *types, ...)
{
    va_list arguments;
    va_start(arguments, types);
    appendVaList(types, arguments);
    va_end(arguments);
}

void Message::appendVaList(const char *types, va_list arguments)
{
    // TODO: avoid segfault if length does not match
    // TODO: avoid segfault if types do not match
    for (int i = 0; types[i] != '\0'; ++i)
    {
        switch (types[i])
        {
            case FLOAT:
            {
                float value = va_arg(arguments, double);
                //std::cout << "Caught a float : " << value;
                appendFloat(value);
                break;
            }
            case INT:
            {
                int value = va_arg(arguments, int);
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
            case DOUBLE:
            {
                double value = va_arg(arguments, double);
                std::cout << "Caught a double : " << value;
                appendDouble(value);
                break;
            }
            case CHAR:
            {
                char value = (char) va_arg(arguments, int);
                //std::cout << "Caught a char : " <<  value;
                appendChar(value);
                break;
            }
            case UNSIGNED_CHAR:
            {
                unsigned char value = (unsigned char) va_arg(arguments, int);
                //std::cout << "Caught a char : " <<  value;
                appendChar(value);
                break;
            }
            case LONG:
            {
                long long int value = va_arg(arguments, long long int);
                //std::cout << "Caught a long long :" << value;
                appendLong(value);
                break;
            }
            case POINTER:
            {
                void *value = va_arg(arguments, void *);
                //std::cout << "Caught a pointer :" << value;
                appendPointer(value);
                break;
            }
            default:
                std::cerr << "Message::" << __FUNCTION__ << ": Unsupported type tag: " << types[i];
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

unsigned int Message::getSize() const
{
    return arguments_.size();
}

const boost::any *Message::getArgument(unsigned int index) const // throw(BadIndexException)
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

void Message::prependFloat(float value)
{
    prepend(boost::any(value));
}

void Message::prependInt(int value)
{
    prepend(boost::any(value));
}

void Message::prependLong(long long int value)
{
    prepend(boost::any(value));
}

void Message::prependStdString(std::string value)
{
    prepend(boost::any(value));
}

void Message::prependBoolean(bool value)
{
    prepend(boost::any(value));
}

void Message::prependChar(char value)
{
    prepend(boost::any(value));
}

void Message::prependDouble(double value)
{
    prepend(boost::any(value));
}

void Message::prependPointer(void *value)
{
    prepend(boost::any(value));
}

void Message::appendFloat(float value)
{
    append(boost::any(value));
}

void Message::appendInt(int value)
{
    append(boost::any(value));
}

void Message::appendLong(long long int value)
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

void Message::appendChar(char value)
{
    append(boost::any(value));
}

void Message::appendUnsignedChar(unsigned char value)
{
    append(boost::any(value));
}

void Message::appendDouble(double value)
{
    append(boost::any(value));
}

void Message::appendPointer(void *value)
{
    append(boost::any(value));
}

int Message::getInt(unsigned int index) const throw(BadArgumentTypeException, BadIndexException)
{
    int value;
    get<int>(index, value);
    return value;
}

long long int Message::getLong(unsigned int index) const throw(BadArgumentTypeException, BadIndexException)
{
    long long int value;
    get<long long int>(index, value);
    return value;
}

double Message::getDouble(unsigned int index) const throw(BadArgumentTypeException, BadIndexException)
{
    double value;
    get<double>(index, value);
    return value;
}

std::string Message::getString(unsigned int index) const throw(BadArgumentTypeException, BadIndexException)
{
    std::string value;
    get<std::string>(index, value);
    return value;
}

float Message::getFloat(unsigned int index) const throw(BadArgumentTypeException, BadIndexException)
{
    float value;
    get<float>(index, value);
    return value;
}

char Message::getChar(unsigned int index) const throw(BadArgumentTypeException, BadIndexException)
{
    char value;
    get<char>(index, value);
    return value;
}

unsigned char Message::getUnsignedChar(unsigned int index) const throw(BadArgumentTypeException, BadIndexException)
{
    unsigned char value;
    get<unsigned char>(index, value);
    return value;
}

bool Message::getBoolean(unsigned int index) const throw(BadArgumentTypeException, BadIndexException)
{
    bool value;
    get<bool>(index, value);
    return value;
}

void *Message::getPointer(unsigned int index) const throw(BadArgumentTypeException, BadIndexException)
{
    void *value;
    get<void *>(index, value);
    return value;
}

void Message::setBoolean(unsigned int index, bool value) throw(BadArgumentTypeException, BadIndexException)
{
    set<bool>(index, value);
}

void Message::setChar(unsigned int index, char value) throw(BadArgumentTypeException, BadIndexException)
{
    set<char>(index, value);
}

void Message::setUnsignedChar(unsigned int index, unsigned char value) throw(BadArgumentTypeException, BadIndexException)
{
    set<unsigned char>(index, value);
}

void Message::setDouble(unsigned int index, double value) throw(BadArgumentTypeException, BadIndexException)
{
    set<double>(index, value);
}

void Message::setFloat(unsigned int index, float value) throw(BadArgumentTypeException, BadIndexException)
{
    set<float>(index, value);
}

void Message::setInt(unsigned int index, int value) throw(BadArgumentTypeException, BadIndexException)
{
    set<int>(index, value);
}

void Message::setLong(unsigned int index, long long int value) throw(BadArgumentTypeException, BadIndexException)
{
    set<long long int>(index, value);
}

void Message::setString(unsigned int index, std::string value) throw(BadArgumentTypeException, BadIndexException)
{
    set<std::string>(index, value);
}

void Message::setPointer(unsigned int index, void *value) throw(BadArgumentTypeException, BadIndexException)
{
    set<void *>(index, value);
}

namespace types
{

bool getArgumentTypeForAny(const boost::any &value, ArgumentType &type)
{
    const std::type_info &actual = value.type();
    if (actual == typeid(void))
        return false;
    if (actual == typeid(bool))
        type = BOOLEAN;
    else if (actual == typeid(char))
        type = CHAR;
    else if (actual == typeid(unsigned char))
        type = UNSIGNED_CHAR;
    else if (actual == typeid(double))
        type = DOUBLE;
    else if (actual == typeid(float))
        type = FLOAT;
    else if (actual == typeid(int))
        type = INT;
    else if (actual == typeid(long long int))
        type = LONG;
    else if (actual == typeid(std::string))
        type = STRING;
    else if (actual == typeid(void *))
        type = POINTER;
    else
    {
        std::cerr << __FUNCTION__ << ": Could not figure out type of value. It's " << actual.name() << std::endl;
        return false;
    }
    return true;
}

} // end of namespace

bool Message::getArgumentType(unsigned int index, ArgumentType &type) const
{
    const boost::any *tmp = getArgument(index);
    if (tmp)
        return types::getArgumentTypeForAny(*tmp, type);
    else
        return false;
}

bool Message::typesMatch(std::string &types) const
{
    if (types.size() != getSize())
        return false;
    unsigned int index = 0;
    ArgumentType actual;
    std::string::iterator iter;
    for (iter = types.begin(); iter < types.end(); ++iter)
    {
        getArgumentType(index, actual);
        if ((*iter) != actual)
            return false;
        ++index;
    }
    return true;
}

bool Message::indexMatchesType(unsigned int index, char type) const
{
    ArgumentType actual;
    if ( ! getArgumentType(index, actual))
        return false;
    return actual == type;
}

std::string Message::getTypes() const
{
    std::string ret;
    for (unsigned int i = 0; i < getSize(); ++i)
    {
        ArgumentType type;
        if (! getArgumentType(i, type))
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

bool Message::operator==(const Message &other) const
{
    if (getTypes() != other.getTypes())
    {
        return false;
    }
    for (unsigned int i = 1; i < getSize(); ++i)
    {
        ArgumentType type;
        getArgumentType(i, type);
        switch (type)
        {
            case BOOLEAN:
                if (getBoolean(i) != other.getBoolean(i))
                    return false;
                break;
            case CHAR:
                if (getChar(i) != other.getChar(i))
                    return false;
                break;
            case UNSIGNED_CHAR:
                if (getUnsignedChar(i) != other.getUnsignedChar(i))
                    return false;
                break;
            case DOUBLE:
                if (getDouble(i) != other.getDouble(i))
                    return false;
                break;
            case FLOAT:
                if (getFloat(i) != other.getFloat(i))
                    return false;
                break;
            case INT:
                if (getInt(i) != other.getInt(i))
                    return false;
                break;
            case LONG:
                if (getLong(i) != other.getLong(i))
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
    for (unsigned int i = 0; i < message.getSize(); ++i)
    {
        ArgumentType type;
        message.getArgumentType(i, type);
        os << (char) type << ":";
        switch (type)
        {
            case BOOLEAN:
                os << message.getBoolean(i);
                break;
            case CHAR:
                os << (unsigned int) message.getChar(i);
                break;
            case UNSIGNED_CHAR:
                os << (unsigned int) message.getUnsignedChar(i);
                break;
            case DOUBLE:
                os << message.getDouble(i);
                break;
            case FLOAT:
                os << message.getFloat(i);
                break;
            case INT:
                os << message.getInt(i);
                break;
            case LONG:
                os << message.getLong(i);
                break;
            case STRING:
                os << "\"" << message.getString(i) << "\"";
                break;
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

} // end of namespace

