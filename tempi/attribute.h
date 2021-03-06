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
 * The Attribute class
 */

#ifndef __TEMPI_ATTRIBUTE_H__
#define __TEMPI_ATTRIBUTE_H__

#include <iostream>
#include <string>
#include "tempi/message.h"
#include "tempi/sharedptr.h"
#include "tempi/documented.h"

namespace tempi {

/**
 * An Attribute is used to store info in a Node and to control its behaviour.
 */
class Attribute : public Documented
{
    public:
        typedef std::tr1::shared_ptr<Attribute> ptr;
        /**
         * Creates an Attribute with a given name, initial value and documentation string.
         * @param name Name for this Attribute
         * @param value Initial value.
         * @param doc Documentation string.
         * @param type_strict Whether or not this Attribute's value is type-strict. (false if its type can change)
         */
        Attribute(const char *name, const Message &value, const char *short_doc="", bool type_strict=true);
        /**
         * Returns the value string of this Attribute.
         */
        const Message &getValue();
        /**
         * Sets the value string of this Attribute.
         * @return False if the type tag of the value is wrong, and this Attribute is type-strict.
         */
        bool setValue(const Message &value);
        /**
         * Checks if this attribute is type-strict.
         * If it is type-strict, one can only set its value to another value with the same type tags.
         */
        bool isTypeStrict() const;
        /**
         * Nodes should call this to make this attribute impossible to change via messages received from other nodes.
         */
        void setMutable(bool is_mutable);
        /**
         * Whether or not users can change the value of this attribute via messages.
         */
        bool getMutable() const;
    private:
        Message value_;
        bool type_strict_;
        bool mutable_;
};

} // end of namespace

#endif // ifndef

