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
#include <cstdarg>
#include "tempi/exceptions.h"
#include <atom/blobvalue.h>

namespace tempi {

/**
 * Type tags for Message arguments.
 */
typedef enum
{
    INVALID = 0,
    BOOLEAN = 'b',
    CHAR = 'c',
    UNSIGNED_CHAR = 'C',
    DOUBLE = 'd',
    FLOAT = 'f',
    INT = 'i',
    UNSIGNED_INT = 'u',
    BANG = '!',
    LONG = 'l',
    STRING = 's',
    BLOB = 'B',
    // TODO: UNICODE = 'u',
    POINTER = 'p'
} AtomType;

class Bang
{
    public:
        Bang() {}
};

namespace types
{
    bool getAtomTypeForAny(const boost::any &value, AtomType &type);
} // end of namespace

/**
 * A Message contains a list of arguments whose types are one of AtomType.
 * Message objects can be stored in a Track, and passed from/to Node inlets and outlets.
 */
class Message
{
    public:
        /**
         * Constructor with no argument.
         */
        Message();

        /**
         * TODO: avoid segfault if length does not match
         * TODO: avoid segfault if types do not match
         */
        Message(const char* types, ...);
        /**
         * TODO: avoid segfault if length does not match
         * TODO: avoid segfault if types do not match
         */
        void append(const char* types, ...);
        //const std::type_info *getType(unsigned int index) const;
        bool getAtomType(unsigned int index, AtomType &type) const;
        //bool setArgument(unsigned int index, boost::any &value);
        unsigned int getSize() const;

        void appendBang();
        void appendBoolean(bool value);
        void appendChar(char value);
        void appendUnsignedChar(unsigned char value);
        void appendUnsignedInt(unsigned int value);
        void appendDouble(double value);
        void appendFloat(float value);
        void appendInt(int value);
        void appendLong(long long int value);
        void appendString(const char *value)
        {
            appendStdString(std::string(value));
        }
        void appendPointer(void *value);
        void appendBlob(atom::BlobValue::ptr value);

        void prependBang();
        void prependBoolean(bool value);
        void prependChar(char value);
        void prependUnsignedChar(unsigned char value);
        void prependUnsignedInt(unsigned int value);
        void prependDouble(double value);
        void prependFloat(float value);
        void prependInt(int value);
        void prependLong(long long int value);
        void prependString(const char *value)
        {
            prependStdString(std::string(value));
        }
        void prependPointer(void *value);
        void prependMessage(const Message &message);
        void prependBlob(atom::BlobValue::ptr value);

        bool getBoolean(unsigned int index) const
            throw(BadAtomTypeException, BadIndexException);
        char getChar(unsigned int index) const
            throw(BadAtomTypeException, BadIndexException);
        unsigned char getUnsignedChar(unsigned int index) const
            throw(BadAtomTypeException, BadIndexException);
        unsigned int getUnsignedInt(unsigned int index) const
            throw(BadAtomTypeException, BadIndexException);
        double getDouble(unsigned int index) const
            throw(BadAtomTypeException, BadIndexException);
        float getFloat(unsigned int index) const
            throw(BadAtomTypeException, BadIndexException);
        int getInt(unsigned int index) const
            throw(BadAtomTypeException, BadIndexException);
        long long int getLong(unsigned int index) const
            throw(BadAtomTypeException, BadIndexException);
        std::string getString(unsigned int index) const
            throw(BadAtomTypeException, BadIndexException);
        //void getString(unsigned int index, std::string &value) const
        //  throw(BadAtomTypeException, BadIndexException);
        void *getPointer(unsigned int index) const
            throw(BadAtomTypeException, BadIndexException);
        atom::BlobValue::ptr getBlob(unsigned int index) const;

        void setBoolean(unsigned int index, bool value)
            throw(BadAtomTypeException, BadIndexException);
        void setChar(unsigned int index, char value)
            throw(BadAtomTypeException, BadIndexException);
        void setUnsignedChar(unsigned int index, unsigned char value)
            throw(BadAtomTypeException, BadIndexException);
        void setUnsignedInt(unsigned int index, unsigned int value)
            throw(BadAtomTypeException, BadIndexException);
        void setDouble(unsigned int index, double value)
            throw(BadAtomTypeException, BadIndexException);
        void setFloat(unsigned int index, float value)
            throw(BadAtomTypeException, BadIndexException);
        void setInt(unsigned int index, int value)
            throw(BadAtomTypeException, BadIndexException);
        void setLong(unsigned int index, long long int value)
            throw(BadAtomTypeException, BadIndexException);
        void setString(unsigned int index, std::string value)
            throw(BadAtomTypeException, BadIndexException);
        void setString(unsigned int index, const char *value)
            throw(BadAtomTypeException, BadIndexException)
        {
            setString(index, std::string(value));
        }
        void setPointer(unsigned int index, void *value)
            throw(BadAtomTypeException, BadIndexException);

        // TODO: deprecate the string version
        bool typesMatch(std::string &types) const;
        bool typesMatch(const char *types) const
        {
            std::string s(types);
            return typesMatch(s);
        }

        bool indexMatchesType(unsigned int index, char type) const;

        std::string getTypes() const;

        // template <typename T>
        // bool appendAny(T value)
        // {
        //     return append(boost::any(value));
        // }

        /**
         * FIXME: seems it's broken.
         */
        bool operator==(const Message &other) const;
        bool operator!=(const Message &other) const;

        Message cloneRange(unsigned int from_index, unsigned int to_index) const
        {
            Message ret;
            for (unsigned int i = 0; i < getSize(); ++i)
            {
                if (i >= from_index && i <= to_index)
                    ret.arguments_.push_back(arguments_[i]);
            }
            return ret;
        }
    private:
        void appendVaList(const char* types, va_list arguments);
        void appendStdString(std::string value);
        void prependStdString(std::string value);
        // Message(std::vector<boost::any> arguments);
        // std::vector<boost::any> &getArguments();
        const boost::any *getArgument(unsigned int index) const; // throw(BadIndexException);
        std::vector<boost::any> arguments_;
        bool append(boost::any value);
        bool prepend(boost::any value);

        template <typename T>
        void get(unsigned int index, T &value) const
            throw(BadAtomTypeException, BadIndexException)
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
                    throw BadAtomTypeException(os.str().c_str());
                }
            else
            {
                std::ostringstream os;
                os << "Message::" << __FUNCTION__ << ": Bad argument index " << index;
                throw BadIndexException(os.str().c_str());
            }
        }

        template <typename T>
        void set(unsigned int index, T value)
            throw(BadAtomTypeException, BadIndexException)
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

