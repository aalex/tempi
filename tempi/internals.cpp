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

#include "tempi/internals.h"
#include "tempi/nodefactory.h"
#include "tempi/library.h"
#include "tempi/osc/osclibrary.h"
#include "tempi/base/baselibrary.h"
#include "tempi/midi/midilibrary.h"
#include "tempi/sampler/samplerlibrary.h"

namespace tempi {
namespace internals {

bool loadInternals(NodeFactory &factory)
{
    librarytools::loadLibrary<midi::MidiLibrary>(factory, "midi.");
    librarytools::loadLibrary<osc::OscLibrary>(factory, "osc.");
    librarytools::loadLibrary<sampler::SamplerLibrary>(factory, "sampler.");
    librarytools::loadLibrary<base::BaseLibrary>(factory, "base.");
}

} // end of namespace
} // end of namespace

