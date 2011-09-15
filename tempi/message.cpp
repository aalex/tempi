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
#include <iostream>
#include <sstream>

namespace tempi
{

// Message::Message(std::vector<boost::any> arguments)
// {
//     arguments_ = arguments;
// }

Message::Message()
{
    // pass
}

bool Message::append(boost::any value)
{
    arguments_.push_back(value);
}

// std::vector<boost::any> &Message::getArguments()
// {
//     return arguments_;
// }

// std::vector<std::type_info> Message::getTypes()
// {
//     std::vector<std::type_info> ret;
//     typename std::vector<boost::any>::iterator iter;
//     for (iter = arguments_.begin(); iter < arguments_.end(); ++iter)
//     {
//         ret.push_back((*iter).type());
//     }
//     return ret;
// }

unsigned int Message::getSize() const
{
    return arguments_.size();
}

// bool Message::setArgument(unsigned int index, boost::any &value)
// {
//     if (index >= getSize())
//     {
//         std::cerr << "Message::" << __FUNCTION__ << ": Index too big: " << index << std::endl;
//         return false;
//     }
// //    else if (value.type() != arguments_[index].type())
// //    {
// //        std::cerr << "Message::" << __FUNCTION__ << ": Wrong type for index " << index << " " << value.type().name() << std::endl;
// //        return false;
// //    }
//     else
//     {
//         arguments_[index] = value;
//         return true;
//     }
// }

boost::any *Message::getArgument(unsigned int index)
{
    if (index >= getSize())
    {
        std::cerr << "Message::" << __FUNCTION__ << ": Index too big: " << index << std::endl;
        return false;
    }
    else
    {
        return &arguments_[index];
    }
}

// const std::type_info *Message::getType(unsigned int index) const
// {
//     if (index >= getSize())
//     {
//         std::cerr << "Message::" << __FUNCTION__ << ": Index too big: " << index << std::endl;
//         return 0;
//     }
//     else
//     {
//         return &(arguments_[index].type());
//     }
// }

bool Message::appendFloat(float value)
{
    append(boost::any(value));
}
bool Message::appendInt(int value)
{
    append(boost::any(value));
}
bool Message::appendLong(long long int value)
{
    append(boost::any(value));
}
bool Message::appendString(std::string value)
{
    append(boost::any(value));
}
bool Message::appendBoolean(bool value)
{
    append(boost::any(value));
}
bool Message::appendChar(char value)
{
    append(boost::any(value));
}
bool Message::appendDouble(double value)
{
    append(boost::any(value));
}

bool Message::getInt(unsigned int index, int &value)
{
    return get<int>(index, value);
}
bool Message::getLong(unsigned int index, long long int &value)
{
    return get<long long int>(index, value);
}
bool Message::getDouble(unsigned int index, double &value)
{
    return get<double>(index, value);
}
bool Message::getString(unsigned int index, std::string &value)
{
    return get<std::string>(index, value);
}
bool Message::getFloat(unsigned int index, float &value)
{
    return get<float>(index, value);
}
bool Message::getChar(unsigned int index, char &value)
{
    return get<char>(index, value);
}
bool Message::getBoolean(unsigned int index, bool &value)
{
    return get<bool>(index, value);
}

bool Message::setBoolean(unsigned int index, bool value)
{
    return set<bool>(index, value);
}
bool Message::setChar(unsigned int index, char value)
{
    return set<char>(index, value);
}
bool Message::setDouble(unsigned int index, double value)
{
    return set<double>(index, value);
}
bool Message::setFloat(unsigned int index, float value)
{
    return set<float>(index, value);
}
bool Message::setInt(unsigned int index, int value)
{
    return set<int>(index, value);
}
bool Message::setLong(unsigned int index, long long int value)
{
    return set<long long int>(index, value);
}
bool Message::setString(unsigned int index, std::string value)
{
    return set<std::string>(index, value);
}

static bool getArgumentTypeForAny(boost::any &value, ArgumentType &type)
{
    const std::type_info &actual = value.type();
    if (actual == typeid(bool))
        type = BOOLEAN;
    else if (actual == typeid(char))
        type = CHAR;
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
    else
    {
        std::cerr << __FUNCTION__ << ": Could not figure out type of value. It's " << actual.name() << std::endl;
        return false;
    }
    return true;
}

bool Message::getArgumentType(unsigned int index, ArgumentType &type)
{
    boost::any *tmp = getArgument(index);
    if (tmp)
        return getArgumentTypeForAny(*tmp, type);
    else
        return false;
}

bool Message::typesMatch(std::string &types)
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

std::string Message::getTypes()
{
    std::ostringstream os;
    for (unsigned int i = 0; i < getSize(); ++i)
    {
        ArgumentType type;
        if (! getArgumentType(i, type))
            std::cerr << "Message::" << __FUNCTION__ << ": Could not get type for arg" << i << std::endl;
        os << type;
    }
    return os.str();
}

} // end of namespace

