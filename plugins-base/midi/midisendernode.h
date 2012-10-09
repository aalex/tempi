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
 * The MidiSenderNode class.
 */
#ifndef __TEMPI_MIDI_MIDISENDERNODE_H__
#define __TEMPI_MIDI_MIDISENDERNODE_H__

#include "tempi/midi.h"
#include "tempi/node.h"

namespace tempi {
namespace plugins_base {

/**
 * Node that sends MIDI messages.
 */
class MidiSenderNode : public Node
{
    public:
        MidiSenderNode();
        ~MidiSenderNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        virtual bool onNodeAttributeChanged(const char *name, const Message &value);
        virtual void doTick()
        {}
        virtual void onInit();
    private:
        midi::Midi *midi_output_;
	int port_;
        /**
         * Opens the given port. Return success.
         */
        bool open(unsigned int port);
        static const char * const EVENTS_INLET;
        static const char * const PORT_ATTR;
};

} // end of namespace
} // end of namespace

#endif // ifndef

