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

#include "plugins-base/music/monodicnode.h"
#include "tempi/utils.h"
#include "tempi/log.h"
#include <iostream>

namespace tempi {
namespace plugins_base {

MonodicNode::MonodicNode() :
    Node(),
    has_open_note_(false),
    open_note_(0)
{
    setShortDocumentation("Generates a stream of MIDI notes that are closed so that it's monophonic.");
    addInlet("0", "Incoming note. (f:pitch)");
    addInlet("close", "Bang to close the last note.");
    addOutlet("0", "Stream of monodic notes that are closed before playing the next one. (f:pitch, f:velocity)");
}

void MonodicNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, "0"))
    {
        if (message.getTypes() != "f")
        {
            Logger::log(ERROR, "MonodicNode expects a float for inlet 0");
            return;
        }
        if (has_open_note_)
            closeNote();
        float note = message.getFloat(0);
        output("0", Message("ff", note, 127.0f)); // pitch velocity
        open_note_ = note;
        has_open_note_ = true;
    }
    else if (utils::stringsMatch(inlet, "close"))
    {
        if (has_open_note_)
        {
            closeNote();
        }
    }
}

void MonodicNode::closeNote()
{
    output("0", Message("ff", open_note_, 0.0f));
    has_open_note_ = false;
}

} // end of namespace
} // end of namespace

