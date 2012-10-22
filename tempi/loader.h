/*
 * Copyright (C) 2011 Alexandre Quessy
 *
 * This file is part of Tempi.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software ither version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tempi.  If not, see
 * <http://www.gnu.org/licenses/>.
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
        /**
         * Retrieves the singleton.
         */
        static Loader& getInstance();
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
        static const char * const PLUGINS_STR;
        // Private constructor
        Loader();
        bool hasPath(const char *path);
        // Dont forget to declare these two. You want to make sure they
        // are unaccessable otherwise you may accidently get copies of
        // your singelton appearing.
        Loader(Loader const&);         // Don't Implement
        void operator=(Loader const&); // Don't implement
};

} // end of namespace

#endif // ifndef

