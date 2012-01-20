/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 *
 * This file is part of Tempi.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of, either version 3 of the License, or
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
 * The TimePosition typedef.
 */
#ifndef __TEMPI_TIMEPOSITION_H__
#define __TEMPI_TIMEPOSITION_H__

namespace tempi
{

/**
 * Time in nanoseconds.
 * There are one billion (1000000000) nanoseconds in a second.
 */
typedef unsigned long long TimePosition;

namespace timeposition
{

static const unsigned long long NS_PER_MS = 1000000L;

/**
 * Converts ms to ns.
 * @return ms * 1000000
 */
TimePosition from_ms(unsigned long long ms);
unsigned long long to_ms(TimePosition time_pos);

} // end of namespace

} // end of namespace

#endif // ifndef


