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

typedef long long Int;
typedef double Float;
typedef unsigned int Index;

/**
 * Type tags for Message arguments.
 */
typedef enum
{
    INVALID = 0,
    BOOLEAN = 'b',
    FLOAT = 'f',
    INT = 'i',
    NONE = 'n',
    STRING = 's',
    BLOB = 'B',
    // TODO: UNICODE = 'u',
    POINTER = 'p'
} AtomType;

class None
{
    public:
        None() {}
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
        //const std::type_info *getType(Index index) const;
        bool getAtomType(Index index, AtomType &type) const;
        //bool setArgument(Index index, boost::any &value);
        Index getSize() const;

        void appendNone();
        void appendBoolean(bool value);
        void appendFloat(Float value);
        void appendInt(Int value);
        void appendString(const char *value)
        {
            appendStdString(std::string(value));
        }
        void appendPointer(void *value);
        void appendBlob(atom::BlobValue::ptr value);

        void prependNone();
        void prependBoolean(bool value);
        void prependFloat(Float value);
        void prependInt(Int value);
        void prependString(const char *value)
        {
            prependStdString(std::string(value));
        }
        void prependPointer(void *value);
        void prependMessage(const Message &message);
        void prependBlob(atom::BlobValue::ptr value);

        bool getBoolean(Index index) const
            throw(BadAtomTypeException, BadIndexException);
        Float getFloat(Index index) const
            throw(BadAtomTypeException, BadIndexException);
        Int getInt(Index index) const
            throw(BadAtomTypeException, BadIndexException);
        std::string getString(Index index) const
            throw(BadAtomTypeException, BadIndexException);
        //void getString(Index index, std::string &value) const
        //  throw(BadAtomTypeException, BadIndexException);
        void *getPointer(Index index) const
            throw(BadAtomTypeException, BadIndexException);
        atom::BlobValue::ptr getBlob(Index index) const;

        void setBoolean(Index index, bool value)
            throw(BadAtomTypeException, BadIndexException);
        void setFloat(Index index, Float value)
            throw(BadAtomTypeException, BadIndexException);
        void setInt(Index index, Int value)
            throw(BadAtomTypeException, BadIndexException);
        void setString(Index index, std::string value)
            throw(BadAtomTypeException, BadIndexException);
        void setString(Index index, const char *value)
            throw(BadAtomTypeException, BadIndexException)
        {
            setString(index, std::string(value));
        }
        void setPointer(Index index, void *value)
            throw(BadAtomTypeException, BadIndexException);

        // TODO: deprecate the string version
        bool typesMatch(std::string &types) const;
        bool typesMatch(const char *types) const
        {
            std::string s(types);
            return typesMatch(s);
        }

        bool indexMatchesType(Index index, char type) const;

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

        Message cloneRange(Index from_index, Index to_index) const
        {
            Message ret;
            for (Index i = 0; i < getSize(); ++i)
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
        const boost::any *getArgument(Index index) const; // throw(BadIndexException);
        std::vector<boost::any> arguments_;
        bool append(boost::any value);
        bool prepend(boost::any value);

        template <typename T>
        void get(Index index, T &value) const
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
                    os << "Message::" << __FUNCTION__ << "<" << typeid(T).name() << ">(" << index << "): Bad template parameter type. Expeceted " << typeid(value).name() << ". Current value is " << value << ". Throwing an exception.";
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
        void set(Index index, T value)
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

