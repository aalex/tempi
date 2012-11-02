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
 * The NameValidator class.
 */

#ifndef __TEMPI_NAME_VALIDATOR_H__
#define __TEMPI_NAME_VALIDATOR_H__

#include <string>
#include <boost/regex.hpp>

namespace tempi {

/**
 * Validates names for named objects.
 */
class NameValidator 
{
    public:
        NameValidator(const char *pattern);
        NameValidator();
        bool setPattern(const char *pattern);
        bool matches(const char * text);
        bool isOk() const;
        std::string getPattern() const;
    private:
        boost::regex pattern_;
        std::string text_pattern_;
        bool ok_;
        static const char * const DEFAULT_PATTERN;
};

} // end of namespace

#endif

