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
 * Include <tr1/memory> or <memory>, depending which is available.
 */
#ifndef __TEMPI_SHAREDPTR_H__
#define __TEMPI_SHAREDPTR_H__

#include <tr1/memory>

namespace tempi
{

template <typename T>
struct SharedPtr : public std::tr1::shared_ptr<T>
{
};

} // end of namespace

#endif // ifndef

