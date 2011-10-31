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
//#include <boost/tuple/tuple.hpp>
//#include <boost/tuple/tuple_io.hpp>
#include <boost/signals2.hpp>
#include <boost/any.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include "tempi/exceptions.h"
#include "tempi/message.h"
//#include "tempi/types.h"

namespace tempi
{

namespace types
{

std::string getPropertyTypeForAny(const boost::any &value);

} // end of namespace

/**
 * A Property holds a value of any type, and triggers a signal every time it is changed.
 * 
 * Can hold a value of a type such as int, float, boost::tuple<float,float>, etc.
 *
 * When its value changes, its signal is triggered with itself as an argument.
 */
class Property
{
    public:
        /** 
         * Typedef for the value_changed_signal_ signal.
         */
        typedef boost::signals2::signal<void (Property &property)> OnChanged;
        /**
         * Constructor with name and value as arguments.
         */
        Property(const char *name, boost::any value, const char *description="");

        /**
         * Returns the type name of the value of this Property.
         */
        std::string getTypeName() const;

        /**
         * Returns the type info for the value of this Property.
         */
        const std::type_info &getType() const;

        /**
         * Checks whether or not the type name of the value of this Property matches a given one.
         */
        bool typeMatches(const char *type) const;

        /**
         * Returns the current value of this Property.
         */
        template <typename T>
        T getValue() const throw(BadArgumentTypeException)
        {
            if (typeid(T) == value_.type())
            {
                return boost::any_cast<T>(value_);
            }
            else
            {
                std::ostringstream os;
                os << "Message::" << __FUNCTION__ << ": Bad argument type. Expected " << value_.type().name() << " but got " << typeid(T).name();
                throw BadArgumentTypeException(os.str().c_str());
            }
        }

        /**
         * Returns the name of this Property.
         */
        const std::string &getName() const;

        /**
         * Returns the description of this Property.
         */
        const std::string &getDescription() const;

        /**
         * Changes the value of this Property and triggers its OnChange signal.
         */
        template <typename T>
        void setValue(T value) throw(BadArgumentTypeException)
        {
            if (value_.type() == typeid(T))
            {
                value_ = value;
                value_changed_signal_(*this);
            }
            else
            {
                std::ostringstream os;
                os << "Message::" << __FUNCTION__ << ": Bad argument type. Expected " << value_.type().name() << " but got " << typeid(T).name();
                throw BadArgumentTypeException(os.str().c_str());
            }
        }

        /**
         * Returns the OnChanged signal of this Property.
         * Useful to register callbacks as slots to it.
         * Callbacks to this signal should return void and accept a reference to a Property object as argument.
         */
        OnChanged &getOnChangedSignal();

    private:
        OnChanged value_changed_signal_;
        std::string name_;
        boost::any value_;
        std::string description_;
};

} // end of namespace

#endif

