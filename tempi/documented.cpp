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

#include "tempi/documented.h"
#include "tempi/namedobject.h"

namespace tempi
{

Documented::Documented(
    const char *name,
    const char *short_documentation,
    const char *long_documentation
    ) :
    NamedObject(name),
    short_documentation_(std::string(short_documentation)),
    long_documentation_(std::string(long_documentation))
{
}

std::string Documented::getShortDocumentation() const
{
    return short_documentation_;
}

std::string Documented::getLongDocumentation() const
{
    return long_documentation_;
}

void Documented::setShortDocumentation(const char *short_documentation)
{
    short_documentation_ = std::string(short_documentation);
}

void Documented::setLongDocumentation(const char *long_documentation)
{
    long_documentation_ = std::string(long_documentation);
}

} // end of namespace

