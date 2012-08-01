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
#include "tempi/midi_output.h"
#include <cstdlib>
#include <iostream>
#include <RtMidi.h>
// #include <portmidi.h>
// #include <porttime.h>

namespace tempi {
namespace midi {

bool MidiOutput::isOpen() const
{
    return opened_;
}

void MidiOutput::enumerateDevices()
{
  ports_count_ = midi_out_->getPortCount();

  //  /* list device information */
  // int i;
  // for (i = 0; i < Pm_CountDevices(); i++) {
  //   const PmDeviceInfo *listinfo = Pm_GetDeviceInfo(i);
  //   std::cout << i << ": " << listinfo->interf << ", " << listinfo->name ;
  //   if (listinfo->input) std::cout << " (input)" << std::endl;
  //   if (listinfo->output) std::cout << " (output)" << std::endl;
  // }
 

    // List inputs.
     std::cout << std::endl << "MIDI output devices: " << ports_count_ << " found." << std::endl;
     std::string portName;
     for (unsigned int i = 0; i < ports_count_; i++)
     {
         try
         {
             portName = midi_out_->getPortName(i);
         }
         catch (RtError &name_error)
         {
             name_error.printMessage();
         }
         std::cout << " * " << i << " : " << portName << '\n';
     }
}

bool MidiOutput::sendMessage(const Message &message) const
{
    if (verbose_)
        std::cout << __FUNCTION__ << "(" << message << ")" << std::endl;
    if (isOpen())
    {
        std::vector<unsigned char> event;
        for (unsigned int i = 0; i < message.getSize(); ++i)
        {
            AtomType type;
            message.getAtomType(i, type);
            if (type == UNSIGNED_CHAR)
                event.push_back(message.getUnsignedChar(i));
        }

         try
         {
             midi_out_->sendMessage(&event);
             return true;
         }
         catch (RtError &error)
         {
             error.printMessage();
             return false;
         }
    }
    else
        return false;
}

MidiOutput::MidiOutput() :
        verbose_(false),
        opened_(false),
        port_(0),
        ports_count_(0),
        client_name_("tempi.midi.out")
{
    // TODO: allow to change the client name using an argument or a attribute
    // RtMidiIn constructor
     try
     {
         midi_out_ = new RtMidiOut(client_name_);
     }
     catch (RtError &error)
     {
         error.printMessage();
         exit(EXIT_FAILURE); //FIXME: do not exit if it fails
     }
     // Check available ports vs. specified.
  ports_count_ = midi_out_->getPortCount();
}

MidiOutput::~MidiOutput()
{
    // if (isOpen())
    //     midi_out_->closePort();
    // delete midi_out_;
}

bool MidiOutput::open(unsigned int port)
{
  ports_count_ = 0;//midi_out_->getPortCount();
    if (port >= ports_count_)
    {
        //TODO: raise excepion !
        // delete midi_out_;
        std::cout << "Invalid input MIDI port!" << std::endl;
        // usage();
        opened_ = false;
        return false;
    }
     try
     {
         midi_out_->openPort(port);
     }
     catch (RtError &error)
     {
         std::cout << "Error opening MIDI port " << port << std::endl;
         error.printMessage();
         opened_ = false;
         return false;
     }
 
    opened_ = true;
    return true;
}

void MidiOutput::setVerbose(bool verbose)
{
    verbose_ = verbose;
}

} // end of namespace
} // end of namespace

