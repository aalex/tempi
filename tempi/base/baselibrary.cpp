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

#include "tempi/base/baselibrary.h"
#include "tempi/base/nop_node.h"
#include "tempi/base/print_node.h"
#include "tempi/base/metro_node.h"
#include "tempi/utils.h"

namespace tempi {
namespace base {

void BaseLibrary::load(NodeFactory &factory, const char *prefix) const
{
    using utils::concatenate;
    factory.registerTypeT<PrintNode>(concatenate(prefix, "print").c_str());
    factory.registerTypeT<NopNode>(concatenate(prefix, "nop").c_str());
    factory.registerTypeT<MetroNode>(concatenate(prefix, "metro").c_str());
}

} // end of namespace
} // end of namespace

