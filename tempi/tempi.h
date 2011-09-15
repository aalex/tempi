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

#include "tempi/filter.h"
#include "tempi/identifier.h"
#include "tempi/loopingplayback.h"
#include "tempi/marker.h"
#include "tempi/message.h"
#include "tempi/node.h"
#include "tempi/pingpongplayback.h"
#include "tempi/playback.h"
#include "tempi/player.h"
#include "tempi/properties.h"
#include "tempi/property.h"
#include "tempi/recorder.h"
#include "tempi/rule.h"
//TODO: #include "tempi/score.h"
#include "tempi/sharedptr.h"
#include "tempi/sink.h"
#include "tempi/sinknode.h"
#include "tempi/slidefilter.h"
#include "tempi/source.h"
#include "tempi/sourcenode.h"
#include "tempi/state.h"
#include "tempi/statemachine.h"
#include "tempi/tempi-version.h"
#include "tempi/timeposition.h"
#include "tempi/timer.h"
#include "tempi/track.h"

#endif // ifndef

