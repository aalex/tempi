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
#include <sstream>
#include <ostream>
#include <string>
#include <typeinfo>
#include <vector>
#include "tempi/exceptions.h"

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

namespace types
{
    bool getArgumentTypeForAny(const boost::any &value, ArgumentType &type);
} // end of namespace

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

        void appendBoolean(bool value);
        void appendChar(char value);
        void appendDouble(double value);
        void appendFloat(float value);
        void appendInt(int value);
        void appendLong(long long int value);
        void appendString(const char *value)
        {
            appendStdString(std::string(value));
        }

        bool getBoolean(unsigned int index) const throw(BadArgumentTypeException, BadIndexException);
        char getChar(unsigned int index) const throw(BadArgumentTypeException, BadIndexException);
        double getDouble(unsigned int index) const throw(BadArgumentTypeException, BadIndexException);
        float getFloat(unsigned int index) const throw(BadArgumentTypeException, BadIndexException);
        int getInt(unsigned int index) const throw(BadArgumentTypeException, BadIndexException);
        long long int getLong(unsigned int index) const throw(BadArgumentTypeException, BadIndexException);
        std::string getString(unsigned int index) const throw(BadArgumentTypeException, BadIndexException);
        //void getString(unsigned int index, std::string &value) const throw(BadArgumentTypeException, BadIndexException);

        void setBoolean(unsigned int index, bool value) throw(BadArgumentTypeException, BadIndexException);
        void setChar(unsigned int index, char value) throw(BadArgumentTypeException, BadIndexException);
        void setDouble(unsigned int index, double value) throw(BadArgumentTypeException, BadIndexException);
        void setFloat(unsigned int index, float value) throw(BadArgumentTypeException, BadIndexException);
        void setInt(unsigned int index, int value) throw(BadArgumentTypeException, BadIndexException);
        void setLong(unsigned int index, long long int value) throw(BadArgumentTypeException, BadIndexException);
        void setString(unsigned int index, std::string value) throw(BadArgumentTypeException, BadIndexException);
        void setString(unsigned int index, const char *value) throw(BadArgumentTypeException, BadIndexException)
        {
            setString(index, std::string(value));
        }

        bool typesMatch(std::string &types) const;
        bool typesMatch(const char *types) const
        {
            std::string s(types);
            return typesMatch(s);
        }

        std::string getTypes() const;

        // template <typename T>
        // bool appendAny(T value)
        // {
        //     return append(boost::any(value));
        // }

        bool operator==(const Message &other) const;
        bool operator!=(const Message &other) const;
    private:

        void appendStdString(std::string value);
        // Message(std::vector<boost::any> arguments);
        // std::vector<boost::any> &getArguments();
        const boost::any *getArgument(unsigned int index) const; // throw(BadIndexException);
        std::vector<boost::any> arguments_;
        bool append(boost::any value);

        template <typename T>
        void get(unsigned int index, T &value) const throw(BadArgumentTypeException, BadIndexException)
        {
            const boost::any *tmp = getArgument(index);
            if (tmp)
                if (tmp->type() == typeid(value))
                {
                    value = boost::any_cast<T>(*tmp);
                }
                else
                {
                    std::ostringstream os;
                    os << "Message::" << __FUNCTION__ << ": Bad argument type " << index << " " << typeid(value).name();
                    throw BadArgumentTypeException(os.str().c_str());
                }
            else
            {
                std::ostringstream os;
                os << "Message::" << __FUNCTION__ << ": Bad argument index " << index;
                throw BadIndexException(os.str().c_str());
            }
        }

        template <typename T>
        void set(unsigned int index, T value) throw(BadArgumentTypeException, BadIndexException)
        {
            if (index >= getSize())
            {
                std::ostringstream os;
                os << "Message::" << __FUNCTION__ << ": Index too big: " << index << std::endl;
                throw BadIndexException(os.str().c_str());
            }
            else
            {
                const boost::any *current = getArgument(index);
                if (current->type() == typeid(value))
                {
                    arguments_[index] = boost::any(value);
                }
                else
                {
                    std::ostringstream os;
                    os << "Message::" << __FUNCTION__ << ": Wrong type " << typeid(value).name() << " for arg: " << index << std::endl;
                    throw BadIndexException(os.str().c_str());
                }
            }
        }
};

std::ostream &operator<<(std::ostream &os, const Message &message);

} // end of namespace

#endif // ifndef

