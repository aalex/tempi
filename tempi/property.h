/*
 * Toonloop
 *
 * Copyright (c) 2010 Alexandre Quessy <alexandre@quessy.net>
 * Copyright (c) 2010 Tristan Matthews <le.businessman@gmail.com>
 *
 * Toonloop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Toonloop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the gnu general public license
 * along with Toonloop.  If not, see <http://www.gnu.org/licenses/>.
 */

/** The Property template class holds a single value. */

#ifndef __TEMPI_PROPERTY_H__
#define __TEMPI_PROPERTY_H__

#include <boost/bind.hpp>
#include <boost/signals2.hpp>
#include <string>

/**
 * Property that can be changed by user interfaces.
 *
 * Can hold a value of a type such as int, float, etc.
 *
 * When its value changes, its signal is triggered with its name
 * and its new value as arguments.
 */

namespace tempi
{

template <typename T>
class Property
{
    public:
        /** 
         * Typedef for the value_changed_signal_ signal.
         */
        typedef boost::signals2::signal<void (std::string, T)> OnChanged;
        //TODO: typedef boost::signals2::signal<void (std::string, boost::any)> OnChanged;
        /**
         * Constructor with name and value as arguments.
         */
        Property(const std::string &name, T value) : 
            name_(name),
            value_(value) {}
        /**
         * Returns the current value of this property.
         */
        T get_value() const { return value_; }
        /**
         * Returns the name of this property.
         */
        const std::string &get_name() const { return name_; }

        void set_value(T value)
        {
            //if (value_ != value)
            value_ = value;
            value_changed_signal_(name_, value_);
        }
        // TODO: make private, and ask to user to use the register_on_changed_slot method.
        OnChanged value_changed_signal_;
    private:
        std::string name_;
        T value_;
};

} // end of namespace

#endif
