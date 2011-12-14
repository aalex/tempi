#include "tempi/sampler/player.h"
#include "tempi/sampler/region.h"
#include "tempi/timer.h"
#include "tempi/sampler/simpleplayback.h"
#include <iostream>

namespace tempi {
namespace sampler {

Player::Player(Region::ptr region) :
    timer_(),
    region_(region),
    speed_(1.0),
    playback_()
{
    playback_.reset(new SimplePlayback());
}

void Player::setRegion(Region::ptr region)
{
    region_ = region;
}

Region::ptr Player::getRegion()
{
    return region_;
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

bool Player::read(Message &result)
{
    if (region_->numberOfEvents() == 0)
    {
        //std::cout << "No point to read." << std::endl;
        return false;
    }
    else
        return playback_.get()->read(*this, result);
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
} // end of namespace

