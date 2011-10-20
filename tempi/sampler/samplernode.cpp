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

#include "tempi/sampler/samplernode.h"

namespace tempi
{

SamplerNode::SamplerNode() :
    Node(),
    track_(new Track()),
    recorder_(new Recorder(track_.get())),
    player_(new Player(track_.get()))
{
    Message recording = Message("b", false);
    addProperty("recording", recording);

    Message playing = Message("b", false);
    addProperty("playing", playing);

    addInlet(); // messages to record
    addOutlet(); // played back messages messages
}

void SamplerNode::onPropertyChanged(const char *name, const Message &value)
{
    const static std::string playing("playing");
    const static std::string recording("recording");

    std::cout << "SamplerNode::" << __FUNCTION__ << ": " << name << " = " << value << std::endl;

    if (playing == name)
        play(value.getBoolean(0));
    else if (recording == name)
        record(value.getBoolean(0));
}

void SamplerNode::play(bool enabled)
{
    if (enabled)
    {
        player_->reset();
    }
}

void SamplerNode::doTick()
{
    if (getProperty("playing").getBoolean(0))
    {
        Message *message = player_->read();
        if (message != 0)
        {
            output(0, *message);
        }
    }
}

void SamplerNode::record(bool enabled)
{
    recorder_->reset();
}

void SamplerNode::processMessage(unsigned int inlet, const Message &message)
{
    bool rec = getProperty("recording").getBoolean(0);
    static unsigned int record_inlet = 0;
    if (inlet == record_inlet)
    {
        if (rec)
        {
            recorder_->add(message);
        }
    }
}

} // end of namespace

