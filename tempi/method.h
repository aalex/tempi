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
 * The Method class.
 */
#ifndef __TEMPI_METHOD_H__
#define __TEMPI_METHOD_H__

#include <boost/signals2.hpp>
#include "tempi/message.h"
#include "tempi/documented.h"
#include "tempi/sharedptr.h"

namespace tempi {

class Method : public Documented
{
    public:
        typedef std::tr1::shared_ptr<Method> ptr;
        typedef boost::signals2::signal<void(Message)> Signal;
        /**
         * Creates a new Method with arguments of a specific type.
         */
        Method(const char *name, const char *short_documentation,
            const char *long_documentation, const char *type);
        /**
         * Creates a new Method with wildcard type. (its arguments can have any types arguments in it)
         */
        Method(const char *name, const char *short_documentation,
            const char *long_documentation);
        /**
         * A virtual destructor makes this class polymorphic.
         */
        virtual ~Method();
        /**
         * Returns the type tags of the arguments to this method.
         * An asterisk ("*") means it can be of any type.
         */
        std::string getType() const;
        /**
         * Returns the boost::signals2::signal<void(Message)>
         */
        Signal &getSignal();
        /**
         * Returns whether or not this signal is type-strict.
         * It depends on its type tag.
         * An asterisk ("*") means it can be of any type.
         */
        bool isTypeStrict() const;
        /**
         * Calls this method with a message as arguments.
         * @return False if the message was not of an OK type or if some error occured while processing it.
         */
        bool call(const Message &message);
    private:
        std::string type_;
        bool type_strict_;
        Signal signal_;
};

} // end of namespace

#endif // ifndef

