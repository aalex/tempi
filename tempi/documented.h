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
 * The Documented class.
 */

#ifndef __TEMPI_DOCUMENTED_H__
#define __TEMPI_DOCUMENTED_H__

#include <string>
#include "tempi/namedobject.h"

namespace tempi
{

/**
 * An Documented is a generic Tempi entity that has a name and some documentation attached to it.
 */
class Documented : public NamedObject
{
    public:
        /**
         * Constructor.
         */
        Documented(
            const char *name = "",
            const char *short_documentation = "",
            const char *long_documentation = "");
        virtual ~Documented() {}
        /**
         * Returns the short documentation string of this Documented.
         */
        std::string getShortDocumentation() const;
        /**
         * Returns the long documentation string of this Documented.
         */
        std::string getLongDocumentation() const;
    protected:
        /**
         * Sets the short documentation string of this Documented.
         */
        void setShortDocumentation(const char *short_documentation);
        /**
         * Sets the long documentation string of this Documented.
         */
        void setLongDocumentation(const char *long_documentation);
    private:
        std::string short_documentation_;
        std::string long_documentation_;
};

} // end of namespace

#endif // ifndef

