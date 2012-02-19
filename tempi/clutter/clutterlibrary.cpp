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

#include "tempi/clutter/clutterlibrary.h"
#include "tempi/clutter/clutterstagenode.h"
#include "tempi/utils.h"
#include "tempi/config.h"

namespace tempi {
namespace clutter {

void ClutterLibrary::load(NodeFactory &factory, const char *prefix) const
{
    using utils::concatenate;
#ifdef HAVE_CLUTTER
    factory.registerTypeT<TempiClutterStageNode>(concatenate(prefix, "stage").c_str());
#endif
}

} // end of namespace
} // end of namespace

