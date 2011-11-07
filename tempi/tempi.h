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
 * Single public header file for tempi.
 *
 * Includes the other header files.
 */

#ifndef __TEMPI_H__
#define __TEMPI_H__

#include <string>

/**
 * @namespace tempi
 *
 * C++ library that deals with timed events.
 *
 * The whole library is within this single namespace.
 */

namespace tempi
{

/**
 * Prints a greeting.
 */
void hello();

/**
 * Returns the version of Tempi as a string.
 */
std::string get_version();

}; // end of namespace

#include "tempi/graph.h"
#include "tempi/identifier.h"
#include "tempi/sampler/loopingplayback.h"
#include "tempi/sampler/marker.h"
#include "tempi/message.h"
#include "tempi/node.h"
#include "tempi/nodefactory.h"
#include "tempi/sampler/pingpongplayback.h"
#include "tempi/sampler/playback.h"
#include "tempi/sampler/player.h"
#include "tempi/sampler/recorder.h"
#include "tempi/fsm/rule.h"
#include "tempi/sharedptr.h"
#include "tempi/mapping/filter.h"
#include "tempi/mapping/slidefilter.h"
#include "tempi/sink.h"
#include "tempi/source.h"
#include "tempi/fsm/state.h"
#include "tempi/fsm/statemachine.h"
#include "tempi/table/table.h"
#include "tempi/tempi-version.h"
#include "tempi/timeposition.h"
#include "tempi/timer.h"
#include "tempi/sampler/track.h"
//#include "tempi/types.h"
//#include "tempi/property.h"

//TODO: #include "tempi/sampler/score.h"

#endif // ifndef

