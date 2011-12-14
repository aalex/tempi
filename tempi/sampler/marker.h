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
 * The Marker class.
 */
#ifndef __TEMPI_MARKER_H__
#define __TEMPI_MARKER_H__

#include <string>
#include "tempi/identifier.h"
#include "tempi/sharedptr.h"

namespace tempi {
namespace sampler {

/**
 * Marker for the beginning of a section in a Track.
 */
class Marker 
{
    public:
        typedef std::tr1::shared_ptr<Marker> ptr;
        Marker(Identifier id);
        Identifier getIdentifier() const;
        const std::string &getName() const;
        void setName(const std::string &name);
    private:
        std::string name_;
        Identifier id_;
};

} // end of namespace
} // end of namespace

#endif // ifndef

