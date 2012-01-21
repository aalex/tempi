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
#include "tempi/sharedptr.h"

namespace tempi
{

class NodeSignal
{
    public:
        typedef std::tr1::shared_ptr<NodeSignal> ptr;
        typedef boost::signals2::signal<void(Message)> Signal;
        NodeSignal(const char *name, const char *doc, const char *type);
        NodeSignal(const char *name, const char *doc);
        ~NodeSignal();
        std::string getName() const;
        std::string getType() const;
        std::string getDocumentation() const;
        Signal &getSignal();
        bool isTypeStrict() const;
        bool trigger(const Message &message);
    private:
        std::string name_;
        std::string doc_;
        std::string type_;
        bool type_strict_;
        Signal signal_;
};

} // end of namespace

#endif // ifndef

