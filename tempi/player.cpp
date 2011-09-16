#include "tempi/player.h"
#include "tempi/track.h"
#include "tempi/timer.h"
#include "tempi/loopingplayback.h"
#include <iostream>

namespace tempi
{

Player::Player(Track *track) :
    timer_(),
    track_(track),
    speed_(1.0),
    playback_()
{
    playback_.reset(new LoopingPlayback());
}

void Player::setTrack(Track *track)
{
    track_ = track;
}

Track *Player::getTrack()
{
    return track_;
}

Timer *Player::getTimer()
{
    return &timer_;
}

void Player::reset()
{
    timer_.reset();
}

bool Player::setPosition(TimePosition position)
{
    return timer_.setPosition(position);
}

Message *Player::read()
{
    if (! track_)
    {
        std::cout << "This Recorder has an invalid Track. Cannot read events." << std::endl;
        return 0;
    }
    else if (track_->numberOfEvents() == 0)
    {
        //std::cout << "No point to read." << std::endl;
        return 0;
    }
    else
        return playback_.get()->read(*this);
}

void Player::setSpeed(double factor)
{
    speed_ = factor;
}

double Player::getSpeed() const
{
    return speed_;
}

void Player::setPlaybackMode(Playback *playback)
{
    playback_.reset(playback);
}

Playback *Player::getPlaybackMode()
{
    return playback_.get();
}

} // end of namespace

