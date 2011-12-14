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
 * The MidiLibrary class.
 */
#ifndef __TEMPI_MIDI_LIBRARY_H__
#define __TEMPI_MIDI_LIBRARY_H__

#include "tempi/library.h"

namespace tempi {
    
/**
 * @namespace tempi::midi
 *
 * MIDI Tempi node types.
 */

namespace midi {

class MidiLibrary : public Library
{
    public:
        virtual void load(NodeFactory &factory, const char *prefix) const;
};

} // end of namespace
} // end of namespace

#endif // ifndef

