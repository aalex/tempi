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
 * The Property class.
 * */

#ifndef __TEMPI_PROPERTY_H__
#define __TEMPI_PROPERTY_H__

#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <boost/signals2.hpp>
#include <boost/any.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include "tempi/exceptions.h"
#include "tempi/message.h"

namespace tempi
{

typedef boost::tuple<float, float> _ff;
typedef boost::tuple<float, float, float> _fff;
typedef boost::tuple<float, float, float, float> _ffff;

namespace types
{

// boost::any makeTuple(const char *typeTags) throw(BadArgumentTypeException)
// {
//     std::string type(typeTags);
//     if (type == "ff")
//         return boost::any(_ff());
//     else if (type == "fff")
//         return boost::any(_fff());
//     else if (type == "ffff")
//         return boost::any(_ffff());
//     else
//     {
//         std::ostringstream os;
//         os << __FUNCTION__ << ": Type tag not supported: " << typeTags;
//         throw BadArgumentTypeException(os.str.c_str());
//     }
// }

std::string getPropertyTypeForAny(const boost::any &value)
{
    if (value.type() == typeid(_ff))
        return "ff";
    if (value.type() == typeid(_fff))
        return "fff";
    if (value.type() == typeid(_ffff))
        return "ffff";
    else
    {
        // std::ostringstream os;
        //os << __FUNCTION__ << ": Type not supported: " << value.type().name();
        throw BadArgumentTypeException("type not supported"); // os.str.c_str());
    }
}

} // end of namespace

/**
 * Property.
 *
 * Can hold a value of a type such as int, float, etc.
 *
 * When its value changes, its signal is triggered with its name
 * and its new value as arguments.
 */
class Property
{
    public:
        /** 
         * Typedef for the value_changed_signal_ signal.
         */
        typedef boost::signals2::signal<void (const Property &property)> OnChanged;
        /**
         * Constructor with name and value as arguments.
         */
        Property(const char *name, boost::any value, const char *description="") :
            name_(name),
            value_(value),
            description_(description)
        {}

        std::string getType() const
        {
            return types::getPropertyTypeForAny(value_);
        }

        bool typeMatches(const char *type) const
        {
            return getType() == type;
        }

        /**
         * Returns the current value of this property.
         */
        const boost::any &getValue() const
        {
            return value_;
        }

        template <typename T>
        T getValue(const boost::any &value) const throw(BadArgumentTypeException)
        {
            if (value.type() == typeid(value))
            {
                return boost::any_cast<T>(value_);
            }
            else
            {
                //std::ostringstream os;
                //os << "Message::" << __FUNCTION__ << ": Bad argument type "; // FIXME: << typeid(value).name();
                //throw BadArgumentTypeException(os.str().c_str());
                throw BadArgumentTypeException("Bad arg type");
            }
        }

        _ff getValue_ff() const throw(BadArgumentTypeException)
        {
            return getValue<_ff>(value_);
        }
        _fff getValue_fff() const throw(BadArgumentTypeException)
        {
            return getValue<_fff>(value_);
        }
        _ffff getValue_ffff() const throw(BadArgumentTypeException)
        {
            return getValue<_ffff>(value_);
        }

        /**
         * Returns the name of this property.
         */
        const std::string &getName() const
        {
            return name_;
        }

        const std::string &getDescription() const
        {
            return description_;
        }

        template <typename T>
        void setValue(T value)
        {
            if (value_.type() == typeid(value))
            {
                value_ = boost::any(value);
                value_changed_signal_(*this);
            }
            else
            {
                //std::ostringstream os;
                //os << "Message::" << __FUNCTION__ << ": Bad argument type " << index << " " << typeid(value).name();
                //throw BadArgumentTypeException(os.str().c_str());
                throw BadArgumentTypeException("Bad arg type");
            }
        }
        // TODO: make private, and ask to user to use the register_on_changed_slot method.
        OnChanged value_changed_signal_;
    private:
        std::string name_;
        boost::any value_;
        std::string description_;
};

} // end of namespace

#endif
