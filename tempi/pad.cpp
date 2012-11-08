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

#include "tempi/pad.h"

namespace tempi
{

Pad::Pad(const char *name, const char *short_documentation, const char *long_documentation) :
    Entity(name, short_documentation, long_documentation)
{
}

void Pad::trigger(const Message &message)
{
    on_triggered_signal_(dynamic_cast<Pad *>(this), message);
}

Pad::TriggeredSignal &Pad::getOnTriggeredSignal()
{
    return on_triggered_signal_;
}

bool Pad::setOwner(Node *node)
{
    this->owner_ = node;
}

Node * Pad::getOwner() const
{
    return this->owner_;
}

} // end of namespace

