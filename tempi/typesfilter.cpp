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

#include "tempi/typesfilter.h"
#include "tempi/exceptions.h"
#include "tempi/log.h"
#include "tempi/utils.h"

namespace tempi {

const char * const TypesFilter::WILDCARD = "*";

TypesFilter::TypesFilter(const char *filter)
{
    this->is_wildcard_ = false;
    this->setFilter(filter);
}

bool TypesFilter::setFilter(const char *filter)
{
    if (TypesFilter::isValidFilter(filter))
    {
        this->filter_ = std::string(filter);
        this->is_wildcard_ = (this->filter_ == TypesFilter::WILDCARD);
        return true;
    }
    else
    {
        std::ostringstream os;
        os << "TypesFilter." << __FUNCTION__ << "(" << filter << "): Invalid type tag.";
        Logger::log(ERROR, os);
        return false;
    }
}

bool TypesFilter::isValidFilter(const std::string &filter)
{
    if (filter == TypesFilter::WILDCARD)
        return true;
    else
        return utils::isValidType(filter.c_str());
}

std::string TypesFilter::getFilter() const
{
    return filter_;
}

bool TypesFilter::matchesFilter(const Message &message) const
{
    if (this->is_wildcard_)
        return true;
    else
    {
        return (message.getTypes() == this->filter_);
    }
}

bool TypesFilter::tryCast(const Message &source, Message &result) const
{
    if (this->is_wildcard_)
    {
        result = source;
        return true;
    }
    else if (this->filter_ == source.getTypes())
    {
        result = source;
        return true;
    }
    else
    {
        return utils::tryAutoCast(source, result, this->filter_);
    }
}

bool TypesFilter::isWildcard() const
{
    return this->is_wildcard_;
}

} // end of namespace

