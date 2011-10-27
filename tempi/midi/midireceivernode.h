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
 * @file
 * The MidiReceiverNode class.
 */
#ifndef __TEMPI_MIDI_MIDIRECEIVERNODE_H__
#define __TEMPI_MIDI_MIDIRECEIVERNODE_H__

#include "tempi/midi/midiinput.h"
#include "tempi/node.h"

namespace tempi
{

namespace midi
{

/**
 * Node that receives MIDI messages.
 */
class MidiReceiverNode : public Node
{
    public:
        MidiReceiverNode();
    protected:
        virtual void doTick();
        virtual void processMessage(unsigned int inlet, const Message &message)
        {}
        virtual void onPropertyChanged(const char *name, const Message &value);
    private:
        MidiInput::ptr midi_input_;
};

} // end of namespace

} // end of namespace

#endif // ifndef

