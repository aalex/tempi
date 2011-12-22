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

#include "tempi/math/mathlibrary.h"
#include "tempi/math/addnode.h"
#include "tempi/math/divnode.h"
#include "tempi/math/iseqnode.h"
#include "tempi/math/multnode.h"
#include "tempi/math/subtractnode.h"
#include "tempi/utils.h"

namespace tempi {
namespace math {

void MathLibrary::load(NodeFactory &factory, const char *prefix) const
{
    using utils::concatenate;
    factory.registerTypeT<AddNode>(concatenate(prefix, "+").c_str());
    factory.registerTypeT<DivNode>(concatenate(prefix, "/").c_str());
    factory.registerTypeT<IsEqualNode>(concatenate(prefix, "==").c_str());
    factory.registerTypeT<SubtractNode>(concatenate(prefix, "-").c_str());
    factory.registerTypeT<MultNode>(concatenate(prefix, "*").c_str());
}

} // end of namespace
} // end of namespace

