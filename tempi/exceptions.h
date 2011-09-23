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
 * Exception classes
 * refer to
 * http://www.boost.org/community/error_handling.html
 * for best practices
 * http://www.boost.org/community/exception_safety.html
 * http://www.parashift.com/c++-faq-lite/exceptions.html#faq-17
 */

#ifndef __TEMPI_EXCEPTIONS_H__
#define __TEMPI_EXCEPTIONS_H__

#include <stdexcept>
#include <string>

namespace tempi
{

class BaseException : public std::runtime_error
{
    public:
        BaseException(const char *error_message) :
            std::runtime_error(error_message)
        {}
        // BaseException(const std::string &error_message) :
        //     std::runtime_error(error_message.c_str())
        // {}
        BaseException() :
            std::runtime_error("")
        {}
};

class BadIndexException : public BaseException
{
    public:
        BadIndexException(const char *error_message) :
            BaseException(error_message)
        {}
};

class BadNodeTypeException : public BaseException
{
    public:
        BadNodeTypeException(const char *error_message) :
            BaseException(error_message)
        {}
};

class BadArgumentTypeException : public BaseException
{
    public:
        BadArgumentTypeException(const char *error_message) :
            BaseException(error_message)
        {}
};

} // end of namespace

#endif // ifndef

