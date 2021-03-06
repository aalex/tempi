/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 * Copyright (C) 2012 Emmanuel Durand
 *
 * This file is part of Tempi-plugins-base.
 *
 * This program is free software; you can redistither version 3 of the License, or
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

#include "plugins-base/midi/midisendernode.h"
#include "plugins-base/midi/mididevicelistermethod.h"
#include "tempi/log.h"
#include "tempi/utils.h"
#include <iostream>

namespace tempi {
namespace plugins_base {

const char * const MidiSenderNode::EVENTS_INLET = "0"; // TODO: rename to events?
const char * const MidiSenderNode::PORT_ATTR = "port";
const char * const MidiSenderNode::ENUMERATE_INLET = "enumerate";
const char * const MidiSenderNode::LIST_METHOD = "list";

MidiSenderNode::MidiSenderNode() :
    Node()
{
    midi_output_ = new midi::Midi();
    this->setShortDocumentation("Sends MIDI messages to a single device.");
    this->addInlet(EVENTS_INLET, "Send MIDI received from this inlet. (list of unsigned characters)");
    this->addInlet(ENUMERATE_INLET, "Prints the list of devices when any message is sent to this inlet.");
    this->addAttribute(Attribute::ptr(new Attribute(PORT_ATTR, Message("i", midi_output_->get_default_output_device_id()), "portmidi device index.")));
    this->addMethod(EntityMethod::ptr(new MidiDeviceListerMethod(LIST_METHOD, "Lists output MIDI devices.",
        "Returns messages with pairs of int and string.", MidiDeviceListerMethod::DESTINATION)));
}

MidiSenderNode::~MidiSenderNode()
{
    if (midi_output_ != 0)
    {
        midi_output_->close_output_device(port_);
        delete midi_output_;
    }
}

void MidiSenderNode::onInit()
{
    // pass
}

bool MidiSenderNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (utils::stringsMatch(name, PORT_ATTR))
    {
        bool success = open((unsigned int) value.getInt(0));
        // TODO: if (! success) { this->setAttribute(PORT_ATTR, Message("i", 0))); return false; }
        return success;
    }
    else
        return true;
}

bool MidiSenderNode::open(unsigned int port)
{
    if (Logger::isEnabledFor(NOTICE))
    {
        std::ostringstream os;
        os << "MidiSenderNode: open port " << port;
        Logger::log(NOTICE, os);
    }
    port_ = port;
    // if (midi_output_ != 0)
    //     delete midi_output_;
    // midi_output_ = new midi::Midi();
    return midi_output_->open_output_device(port);
}

// TODO: output the list of devices upon query

void MidiSenderNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, ENUMERATE_INLET))
    {
        midi_output_->enumerate_devices();
        return;
    }
    if (! utils::stringsMatch(inlet, EVENTS_INLET))
        return;
    if (midi_output_->is_open(port_))
    {
        std::vector<unsigned char> event;
        for (unsigned int i = 0; i < message.getSize(); ++i)
        {
            AtomType type;
            message.getAtomType(i, type);
            if (type == INT)
                event.push_back(message.getInt(i));
        }
        if (event.size() == 3)
        {
            bool ok = midi_output_->send_message_to_output(port_, event[0], event[1], event[2]);
            if (! ok)
            {
                std::ostringstream os;
                os << "MidiSenderNode::" << __FUNCTION__ << ": Error sending " << message;
                Logger::log(WARNING, os);
            }
        }
        else
        {
            std::ostringstream os;
            os << "MidiSenderNode::" << __FUNCTION__ << ": Malformed incomming message. Cannot send " << message;
            Logger::log(WARNING, os);
        }
    }
    else
    {
        std::ostringstream os;
        os << "MidiSenderNode::" << __FUNCTION__ << ": MIDI output is not open. Cannot send " << message;
        Logger::log(WARNING, os);
    }
}

} // end of namespace
} // end of namespace

