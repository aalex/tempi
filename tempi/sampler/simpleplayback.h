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

/**
 * @file
 * The SimplePlayback class.
 */
#ifndef __TEMPI_SIMPLEPLAYBACK_H__
#define __TEMPI_SIMPLEPLAYBACK_H__

#include "tempi/sampler/playback.h"
#include "tempi/timeposition.h"

namespace tempi {
namespace sampler {

/**
 * A SimplePlayback plays each event of a Region once from start to end, and goes back to its beginning once at its end.
 */
class SimplePlayback : public Playback
{
    public:
        virtual bool read(Player &player, Message &result);
    private:
        TimePosition previous_timeposition_played_;
};

} // end of namespace
} // end of namespace

#endif // ifndef

