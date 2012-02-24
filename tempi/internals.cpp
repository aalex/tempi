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

#include "tempi/internals.h"
#include "tempi/nodefactory.h"
#include "tempi/library.h"
//#include "plugins-base/baselibrary.h"
#include "tempi/loader.h"
#include "tempi/config.h"

namespace tempi {
namespace internals {

bool loadInternals(NodeFactory &factory)
{
    using librarytools::loadLibrary;
    Loader& loader = Loader::getInstance();
    loader.addPath(".");
    loader.addPath("/usr/local/lib/" TEMPI_PLUGINS_DIR); // FIXME
    loader.addPath("/usr/local/lib/"); // FIXME
    loader.addPath("/usr/lib/" TEMPI_PLUGINS_DIR); // FIXME
    loader.addPath("./plugins-base/.libs");
    loader.addPath("../plugins-base/.libs");
    loader.addPath("../../plugins-base/.libs");
    return loader.load(factory, "base");
    //loadLibrary<base::BaseLibrary>(factory, "base.");
    return true;
}

bool loadInternals(NodeFactory::ptr factory)
{
    NodeFactory *ptr = factory.get();
    if (ptr == 0)
    {
        std::cerr << "tempi::internals::" << __FUNCTION__ << ": factory is a null pointer" << std::endl;
        return false;
    }
    return loadInternals(*ptr);
}

} // end of namespace
} // end of namespace

