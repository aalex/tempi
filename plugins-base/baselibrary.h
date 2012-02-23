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
 * The BaseLibrary class.
 */
#ifndef __TEMPI_BASELIBRARY_H__
#define __TEMPI_BASELIBRARY_H__

#include "tempi/library.h"

namespace tempi { 

/**
 * @namespace tempi::base
 *
 * Tempi-plugins-base node types.
 */

namespace base {

class BaseLibrary : public Library
{
    public:
        virtual void load(NodeFactory &factory, const char *prefix) const;
};

} // end of namespace
} // end of namespace

#endif // ifndef
