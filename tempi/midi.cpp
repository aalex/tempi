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
#include "tempi/midi.h"
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>

namespace tempi {
namespace midi {

//initlization of the singleton to NULL
MidiScheduler * Midi::sched_ = NULL;
int Midi::streams = 0;

Midi::Midi()
{
    if (sched_ != NULL) 
        return;
    sched_ = new MidiScheduler();
    // TODO: replace by a shared pointer
}

Midi::~Midi()
{
    if (streams == 0)
    {
        delete sched_;
        if (sched_ != NULL)
            std::cout << __FUNCTION__ << ": sched not null" << std::endl;
    }
}

int Midi::get_default_output_device_id()
{
    return Pm_GetDefaultOutputDeviceID();
}

int Midi::get_default_input_device_id()
{
    return Pm_GetDefaultInputDeviceID();
}

bool Midi::open_input_device(int id)
{
    PmStream *stream = sched_->add_input_stream (id);
    if (stream != NULL)
    {
        streams++;
        openned_streams_[id] = stream;
        return true;
    }
    else
        return false;
}

bool Midi::open_output_device(int id)
{
    PmStream *stream = sched_->add_output_stream (id);
    if (stream != NULL)
    {
        streams++;
        openned_streams_[id] = stream;
        return true;
    }
    else
        return false;
}


bool Midi::is_open(int id)
{
    //checking if the current instance has openned the device
    if (openned_streams_.find(id) == openned_streams_.end())
        return false;
    else
        return true;
}

void Midi::close_input_device(int id)
{
    std::map<int, PmStream *>::iterator it = openned_streams_.find(id);
    if (it == openned_streams_.end())
        return;
    if (sched_->remove_input_stream (it->second))
        streams--;
}

void Midi::close_output_device(int id)
{
    std::map<int, PmStream *>::iterator it = openned_streams_.find(id);
    if (it == openned_streams_.end())
        return;
  
    if (sched_->remove_output_stream (it->second))
        streams--;
}

bool Midi::is_queue_empty(int id)
{
    std::map<int, PmStream *>::iterator it = openned_streams_.find(id);
    if (it == openned_streams_.end())
    {
      //the queue is actually not empty but the id is not managed by this instance
      return false;
    }
    return sched_->is_queue_empty (it->second);
}

bool Midi::send_message_to_output(int id, unsigned char status, unsigned char data1, unsigned char data2)
{
    std::map<int, PmStream *>::iterator it = openned_streams_.find(id);
    if ( it == openned_streams_.end())
    {
        return false;
    }
    return sched_->push_message(it->second,status,data1,data2);
}

// return empty vector if not accessible or <status> <data1> <data2> id success
std::vector<unsigned char> 
Midi::poll(int id)
{
    std::vector<unsigned char> message;
    std::map<int, PmStream *>::iterator it = openned_streams_.find(id);
    if ( it == openned_streams_.end())
    {
        //the queue is not accessible
        return message;
    }
    PmEvent event = sched_->poll (it->second);
    message.push_back ((unsigned char)Pm_MessageStatus(event.message));
    message.push_back ((unsigned char)Pm_MessageData1(event.message));
    message.push_back ((unsigned char)Pm_MessageData2(event.message));

    return message;
} 

void Midi::enumerate_devices() const
{
    // TODO: return a vector<string>
    /* list device information */
    int i;
    for (i = 0; i < Pm_CountDevices(); i++)
    {
        const PmDeviceInfo *listinfo = Pm_GetDeviceInfo(i);
        std::cout << i << ": " << listinfo->interf << ", " << listinfo->name ;
        if (listinfo->input)
            std::cout << " (input)" << std::endl;
        if (listinfo->output)
            std::cout << " (output)" << std::endl;
    }
}

std::map<int, std::string> Midi::listDevices(Midi::DeviceDirection direction)
{
    std::map<int, std::string> ret;
    int i;
    for (i = 0; i < Pm_CountDevices(); i++)
    {
        const PmDeviceInfo *listinfo = Pm_GetDeviceInfo(i);
        std::ostringstream os;
        os << listinfo->interf << ", " << listinfo->name ;
        if (listinfo->input && direction == SOURCE)
            ret[i] = os.str();
        if (listinfo->output && direction == DESTINATION)
            ret[i] = os.str();
    }
    return ret;
}

} // end of namespace
} // end of namespace

