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

#ifndef __SAMPLER_H__
#define __SAMPLER_H__

#include "tempi/tempi.h"
#include <tr1/memory>
#include "particlegenerator.h"

struct Sampler
{
    public:
        Sampler();
        void startRecording();
        void stopRecording();
        bool isRecording();
        tempi::sampler::Player *getPlayer();
        tempi::sampler::Recorder *getRecorder();
        ParticleGenerator *getGenerator();
        void setColor(int r, int g, int b);
    private:
        tempi::sampler::Region::ptr track_;
        tempi::sampler::Recorder::ptr recorder_;
        tempi::sampler::Player::ptr player_;
        bool recording_;
        ParticleGenerator generator_;
};

#endif

