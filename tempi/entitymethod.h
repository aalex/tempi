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
 * The EntityMethod class.
 */
#ifndef __TEMPI_ENTITYMETHOD_H__
#define __TEMPI_ENTITYMETHOD_H__

#include "tempi/message.h"
#include "tempi/documented.h"
#include "tempi/sharedptr.h"

namespace tempi {

class EntityMethod : public Documented
{
    public:
        typedef std::tr1::shared_ptr<EntityMethod> ptr;
        /**
         * Creates a new EntitySignal with messages of a specific type.
         */
        EntityMethod(const char *name, const char *short_documentation,
            const char *long_documentation, const char *arguments_type, const char *return_type);
        /**
         * Makes this class virtual.
         */
        ~EntityMethod();
        /**
         * Returns the type tags of the arguments for this method.
         * An asterisk ("*") means it can be of any type.
         */
        std::string getArgumentsType() const;
        /**
         * Returns the type tags of the return value of this method.
         * An asterisk ("*") means it can be of any type.
         */
        std::string getReturnType() const;
        /**
         * Calls this method
         * @return False if the message was not of an OK type.
         */
        bool call(const Message &arguments, Message & return_value);

    protected:
        /**
         * Derived classes must implement this method.
         */
        virtual bool doCall(const Message &arguments, Message & return_value) = 0;
    private:
        std::string return_type_;
        std::string arguments_type_;
};

} // end of namespace

#endif // ifndef

