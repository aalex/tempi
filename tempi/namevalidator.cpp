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

#include "tempi/namevalidator.h"

namespace tempi {

const char * const NameValidator::DEFAULT_PATTERN = "^(\\w|-|_)+$";

NameValidator::NameValidator()
{
    this->pattern_ = std::string(DEFAULT_PATTERN);
    this->text_pattern_ = DEFAULT_PATTERN;
    this->ok_ = true;
}

NameValidator::NameValidator(const char *pattern) :
    ok_(false)
{
    bool ok = this->setPattern(pattern);
    (void) ok;
}

bool NameValidator::setPattern(const char *pattern)
{
    try
    {
        this->pattern_ = std::string(pattern);
        this->text_pattern_ = pattern;
        this->ok_ = true;
    }
    catch (const boost::regex_error& e)
    {
        std::cerr << __FUNCTION__ << ": " << e.what() << std::endl;
    }
    return this->ok_;
}

bool NameValidator::isOk() const
{
    return this->ok_;
}

std::string NameValidator::getPattern() const
{
    return this->text_pattern_;
}

bool NameValidator::matches(const char *text)
{
    try
    {
        boost::cmatch what;
        if (boost::regex_match(text, what, this->pattern_))
            return true;
        else
            return false;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "NameValidator::" << __FUNCTION__ << ": " << e.what() << std::endl;
        return false;
    }
    return false;
}

} // end of namespace

