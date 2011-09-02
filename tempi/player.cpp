#include "player.h"
#include <iostream>

namespace tempi
{

Player::Player(Track *track) :
    timer_(),
    track_(track),
    empty_(),
    speed_(1.0)
{
    // pass
}

void Player::setTrack(Track *track)
{
    track_ = track;
}

Track *Player::getTrack()
{
    return track_;
}

void Player::reset()
{
    timer_.reset();
}

boost::any *Player::readLoop()
{
    if (! track_)
    {
        std::cout << "This Recorder has an invalid Track. Cannot read events." << std::endl;
        return &empty_;
    }

    TimeStamp duration = track_->getDuration();
    TimeStamp elapsed = timer_.elapsed();
    elapsed *= speed_; // FIXME: speed factor is broken here.
    //std::cout << __FUNCTION__ << std::endl;
    if (track_->numberOfEvents() == 0)
    {
        //std::cout << "No point to read." << std::endl;
        return 0;
    }
    else if (duration == 0L)
    {
        //std::cout << "Read first point." << std::endl;
        return track_->getFirst();
    }
    else if (duration == elapsed)
    {
        //std::cout << "Read last point." << std::endl;
        return  track_->getLast();
    }
    else
    {
        //std::cout << "duration: " << getDuration() << std::endl;
        // FIXME: using the modulo here should not be mandatory
        TimeStamp cursor = elapsed % duration;
        //std::cout << "elapsed modulo duration: " << cursor << std::endl;
        return track_->getClosest(cursor);
    }
}

void Player::setSpeed(double factor)
{
    speed_ = factor;
}

double Player::getSpeed() const
{
    return speed_;
}

} // end of namespace

