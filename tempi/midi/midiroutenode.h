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
 * The MidiRouterNode class.
 */
#ifndef __TEMPI_MIDI_MIDIROUTENODE_H__
#define __TEMPI_MIDI_MIDIROUTENODE_H__

#include "tempi/node.h"

namespace tempi {
namespace midi {

/**
 * Node that routes MIDI messages according to their type.
 */
class MidiRouteNode : public Node
{
    public:
        MidiRouteNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        // virtual void onAttributeChanged(const char *name, const Message &value);
};

} // end of namespace
} // end of namespace

#endif // ifndef

