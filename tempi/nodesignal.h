/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 *
 * This file is part of Tempi.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of, either version 3 of the License, or
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
 * The NodeSignal class.
 */
#ifndef __TEMPI_NODESIGNAL_H__
#define __TEMPI_NODESIGNAL_H__

#include <boost/signals2.hpp>
#include "tempi/message.h"
#include "tempi/documented.h"
#include "tempi/sharedptr.h"

namespace tempi {

class NodeSignal : public Documented
{
    public:
        typedef std::tr1::shared_ptr<NodeSignal> ptr;
        typedef boost::signals2::signal<void(Message)> Signal;
        /**
         * Creates a new NodeSignal with messages of a specific type.
         */
        NodeSignal(const char *name, const char *short_documentation,
            const char *long_documentation, const char *type);
        /**
         * Creates a new NodeSignal with wildcard type. (its message can have any types arguments in it)
         */
        NodeSignal(const char *name, const char *short_documentation,
            const char *long_documentation);
        /**
         * A virtual destructor makes this class polymorphic.
         */
        virtual ~NodeSignal();
        /**
         * Returns the type tags of the messages triggered by this signal.
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
         * Triggers the signal with a message.
         * @return False if the message was not of an OK type.
         */
        bool trigger(const Message &message);
    private:
        std::string type_;
        bool type_strict_;
        Signal signal_;
};

} // end of namespace

#endif // ifndef

