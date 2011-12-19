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

#include "sampler.h"
#include "tempi/sampler/simpleplayback.h"

Sampler::Sampler() :
    track_(new tempi::sampler::Region)
{
    recorder_.reset(new tempi::sampler::Recorder(track_));
    player_.reset(new tempi::sampler::Player(track_));
    player_->setPlaybackMode(new tempi::sampler::SimplePlayback);
    recording_ = false;
}

ParticleGenerator *Sampler::getGenerator()
{
    return &generator_;
}

tempi::sampler::Player *Sampler::getPlayer()
{
    return player_.get();
}

tempi::sampler::Recorder *Sampler::getRecorder()
{
    return recorder_.get();
}

void Sampler::setColor(int r, int g, int b)
{
    generator_.setColor(r, g, b);
}

void Sampler::startRecording()
{
    recording_ = true;
    track_.reset(); // clears the track
    recorder_.get()->start();
}

void Sampler::stopRecording()
{
     recording_ = false;
    recorder_.get()->stop();
}

bool Sampler::isRecording()
{
     return recording_;
}

