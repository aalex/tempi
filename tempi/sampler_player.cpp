/*
 * Copyright (C) 2011 Alexandre Quessy
 *
 * This file is part of Tempi.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software ither version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tempi.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include "tempi/sampler_player.h"
#include "tempi/sampler_region.h"
#include "tempi/timer.h"
#include "tempi/sampler_simpleplayback.h"
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

bool Player::read(std::vector<Message> &result)
{
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

