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

#include "plugins-base/midi/midireceivernode.h"
#include "plugins-base/midi/mididevicelistermethod.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const MidiReceiverNode::EVENTS_OUTLET = "0";
const char * const MidiReceiverNode::ENUMERATE_INLET = "enumerate";
const char * const MidiReceiverNode::PORT_ATTR = "port";
const char * const MidiReceiverNode::LIST_METHOD = "list";

MidiReceiverNode::MidiReceiverNode() :
    Node(),
    did_print_unitialized_message_(false)
{
    midi_input_ = new midi::Midi();
    this->setShortDocumentation("Receives MIDI messages from a single device.");
    this->addOutlet(EVENTS_OUTLET, "MIDI messages (unsigned characters) flow through this outlet.");
    this->addInlet(ENUMERATE_INLET, "Prints the list of devices when any message is sent to this inlet.");
    this->addAttribute(Attribute::ptr(new Attribute(PORT_ATTR, Message("i", midi_input_->get_default_input_device_id()), "portmidi device index.")));
    this->addMethod(EntityMethod::ptr(new MidiDeviceListerMethod(LIST_METHOD, "Lists input MIDI devices.",
        "Returns messages with pairs of int and string.", MidiDeviceListerMethod::SOURCE)));
}

MidiReceiverNode::~MidiReceiverNode()
{
    midi_input_->close_input_device(port_);
    if (midi_input_ != 0)
        delete midi_input_;
}

bool MidiReceiverNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    //std::cout << "MidiReceiverNode::" << __FUNCTION__ << "(" << name << ", " << value << ")" << std::endl;
    if (utils::stringsMatch(name, PORT_ATTR))
    {
        bool success = open(value.getInt(0));
        // TODO: if (! success) { this->setAttribute(PORT_ATTR, Message("i", 0))); return false; }
        return success;
    }
    else
    {
        return true;
    }
}

bool MidiReceiverNode::open(unsigned int port)
{
    if (Logger::isEnabledFor(NOTICE))
    {
        std::ostringstream os;
        os << "MidiReceiverNode: open port " << port;
        Logger::log(NOTICE, os);
    }
    port_ = port;
    // if (midi_input_ != 0)
    //     delete midi_input_;
    // midi_input_ = new midi::Midi();
    return midi_input_->open_input_device(port);
}

void MidiReceiverNode::onInit()
{
    // pass
}

void MidiReceiverNode::doTick()
{
    if (! midi_input_->is_open(port_))
    {
        if (! this->did_print_unitialized_message_)
        {
            std::ostringstream os;
            os << "MidiReceiverNode::" << __FUNCTION__ <<
                "(): MidiInput is not initialized. Please specifiy a port number.";
            Logger::log(WARNING, os);
            this->did_print_unitialized_message_ = true;
        }
        return;
    }

    while (! midi_input_->is_queue_empty(port_))
    {
        std::vector<unsigned char> midi_message = midi_input_->poll(port_);
        Message to_output_message;
        if (midi_message.size() == 3)
        {
            to_output_message.appendInt(midi_message[0]);
            to_output_message.appendInt(midi_message[1]);
            to_output_message.appendInt(midi_message[2]);
        }
        else if (midi_message.size() == 2)
        {
            to_output_message.appendInt(midi_message[0]);
            to_output_message.appendInt(midi_message[1]);
        }
        output(EVENTS_OUTLET, to_output_message);
    }
}

void MidiReceiverNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, ENUMERATE_INLET))
    {
        midi_input_->enumerate_devices();
    }
}

} // end of namespace
} // end of namespace

