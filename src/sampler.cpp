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
#include "tempi/pingpongplayback.h"

Sampler::Sampler()
{
    recorder_.reset(new tempi::Recorder(&track_));
    player_.reset(new tempi::Player(&track_));
    player_.get()->setPlaybackMode(new tempi::PingPongPlayback());
    recording_ = false;
}

ParticleGenerator *Sampler::getGenerator()
{
    return &generator_;
}

tempi::Player *Sampler::getPlayer()
{
    return player_.get();
}

tempi::Recorder *Sampler::getRecorder()
{
    return recorder_.get();
}

void Sampler::startRecording()
{
    recording_ = true;
    track_.reset(); // clears the track
    recorder_.get()->reset();
}

void Sampler::stopRecording()
{
     recording_ = false;
}

bool Sampler::isRecording()
{
     return recording_;
}

