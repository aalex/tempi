#include "recorder.h"
#include <iostream>

namespace tempi
{

Recorder::Recorder(Track *track)
{
    track_ = track;
}

void Recorder::setTrack(Track *track)
{
    track_ = track;
}

Track *Recorder::getTrack()
{
    return track_;
}

void Recorder::reset()
{
    timer_.reset();
}

void Recorder::add(boost::any value)
{
    if (track_)
        track_->add(timer_.elapsed(), value);
    else
        std::cout << "This Recorder has an invalid Track. Cannot add event." << std::endl;
}

} // end of namespace

