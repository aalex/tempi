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
 * The CounterNode class.
 */

#ifndef __TEMPI_COUNTERNODE_H__
#define __TEMPI_COUNTERNODE_H__

#include "tempi/node.h"

namespace tempi { namespace base {

/**
 * A CounterNode is a Node that counts.
 */
class CounterNode : public Node
{
    public:
        static const char * const PROP_INCREMENT;
        static const char * const PROP_COUNT;
        CounterNode();
    protected:
        virtual void processMessage(unsigned int inlet, const Message &message);
        virtual void onPropertyChanged(const char *name, const Message &value);
    private:
        int count_;
        int increment_;
};

} // end of namespace
} // end of namespace

#endif // ifndef

