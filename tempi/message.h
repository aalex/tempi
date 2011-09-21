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
 * The Message class
 */

#ifndef __TEMPI_MESSAGE_H__
#define __TEMPI_MESSAGE_H__

#include <boost/any.hpp>
#include <iostream>
#include <ostream>
#include <string>
#include <typeinfo>
#include <vector>

namespace tempi
{

/**
 * Type tags for Message arguments.
 */
typedef enum
{
    BOOLEAN = 'b',
    CHAR = 'c',
    DOUBLE = 'd',
    FLOAT = 'f',
    INT = 'i',
    LONG = 'l',
    STRING = 's'
    //TODO: POINTER = 'p'
} ArgumentType;

/**
 * A Message contains a list of arguments whose types are one of ArgumentType.
 * Message objects can be stored in a Track, and passed from/to Node inlets and outlets.
 */
class Message
{
    public:
        /**
         * Constructor with no argument.
         */
        Message();
        //const std::type_info *getType(unsigned int index) const;
        bool getArgumentType(unsigned int index, ArgumentType &type) const;
        //bool setArgument(unsigned int index, boost::any &value);
        unsigned int getSize() const;

        bool appendBoolean(bool value);
        bool appendChar(char value);
        bool appendDouble(double value);
        bool appendFloat(float value);
        bool appendInt(int value);
        bool appendLong(long long int value);
        bool appendString(std::string value);

        bool getBoolean(unsigned int index, bool &value) const;
        bool getChar(unsigned int index, char &value) const;
        bool getDouble(unsigned int index, double &value) const;
        bool getFloat(unsigned int index, float &value) const;
        bool getInt(unsigned int index, int &value) const;
        bool getLong(unsigned int index, long long int &value) const;
        bool getString(unsigned int index, std::string &value) const;

        bool setBoolean(unsigned int index, bool value);
        bool setChar(unsigned int index, char value);
        bool setDouble(unsigned int index, double value);
        bool setFloat(unsigned int index, float value);
        bool setInt(unsigned int index, int value);
        bool setLong(unsigned int index, long long int value);
        bool setString(unsigned int index, std::string value);

        bool typesMatch(std::string &types) const;
        bool typesMatch(const char *types) const
        {
            std::string s(types);
            return typesMatch(s);
        }

        std::string getTypes();

        // template <typename T>
        // bool appendAny(T value)
        // {
        //     return append(boost::any(value));
        // }
    private:
        // Message(std::vector<boost::any> arguments);
        // std::vector<boost::any> &getArguments();
        const boost::any *getArgument(unsigned int index) const;
        std::vector<boost::any> arguments_;
        bool append(boost::any value);

        template <typename T>
        bool get(unsigned int index, T &value) const
        {
            const boost::any *tmp = getArgument(index);
            if (tmp)
                if (tmp->type() == typeid(value))
                {
                    value = boost::any_cast<T>(*tmp);
                    return true;
                }
                else
                    return false;
            else
                return false;
        }

        template <typename T>
        bool set(unsigned int index, T value)
        {
            if (index >= getSize())
            {
                std::cerr << "Message::" << __FUNCTION__ << ": Index too big: " << index << std::endl;
                return false;
            }
            else
            {
                const boost::any *current = getArgument(index);
                if (current->type() == typeid(value))
                {
                    arguments_[index] = boost::any(value);
                    return true;
                }
                else
                {
                    std::cerr << "Message::" << __FUNCTION__ << ": Wrong type for arg: " << index << std::endl;
                    return false;
                }

            }
        }
};

std::ostream &operator<<(std::ostream &os, const Message &message);

} // end of namespace

#endif // ifndef

