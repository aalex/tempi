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
 * The Library class.
 */
#ifndef __TEMPI_LIBRARY_H__
#define __TEMPI_LIBRARY_H__

#include "tempi/nodefactory.h"
#include "tempi/sharedptr.h"

namespace tempi
{

/**
 * A Library is a collection of Node types.
 */
class Library
{
    public:
        typedef std::tr1::shared_ptr<Library> ptr;
        /**
         * Loads node types for a library.
         */
        virtual void load(NodeFactory &factory, const char *prefix) const = 0;
};

namespace librarytools
{

template <class T>
void loadLibrary(NodeFactory &factory, const char *prefix)
{
    Library::ptr library(new T);
    library->load(factory, prefix);
}

} // end of namespace

} // end of namespace

#endif // ifndef

