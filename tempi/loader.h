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
 * Logging utilities
 */
#ifndef __TEMPI_LOADER_H__
#define __TEMPI_LOADER_H__

#include <string>
#include "tempi/nodefactory.h"
#include <boost/filesystem/path.hpp>

namespace tempi {

class Loader
{
    public:
        typedef void (*SetupFunc)(void *);
        Loader();
        bool addPath(const char *path);
        bool load(NodeFactory &factory, const char *name);
        bool isLoaded(const char *name);
    private:
        static std::vector<std::string> loaded_;
        std::vector<std::string> paths_;
        static const char * const FILE_PREFIX;
        static const char * const FILE_SUFFIX;
        static const char * const FUNC_PREFIX;
        static const char * const FUNC_SUFFIX;
        bool hasPath(const char *path);
};

} // end of namespace

#endif // ifndef

