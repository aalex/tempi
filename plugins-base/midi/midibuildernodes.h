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
 * The AbstractMidiEventBuilderNode Class
 */
#ifndef __TEMPI_MIDI_MIDIBUILDERNODE_H__
#define __TEMPI_MIDI_MIDIBUILDERNODE_H__

#include "plugins-base/midi/abstractmidifilter.h"

namespace tempi {
namespace plugins_base {

/**
 * Base class for building MIDI events
 */
class NoteBuilderNode : public AbstractMidiEventBuilderNode
{
    public:
        NoteBuilderNode();
    private:
        int note_;
        int velocity_;
        virtual bool buildMidiEvent(const std::vector<int>& ints, Message &result);
};

/**
 * MIDI control messages
 */
class ControlBuilderNode : public AbstractMidiEventBuilderNode
{
    public:
        ControlBuilderNode();
    private:
        int value_;
        virtual bool buildMidiEvent(const std::vector<int>& ints, Message &result);
    protected:
        static const char * const CTL_ATTR; // set controller number attribute
};

/**
 * Program change messages
 */
class ProgramChangeBuilderNode : public AbstractMidiEventBuilderNode
{
    public:
        ProgramChangeBuilderNode();
    private:
        int program_;
        virtual bool buildMidiEvent(const std::vector<int>& ints, Message &result);
};
} // end of namespace
} // end of namespace

#endif // ifndef

