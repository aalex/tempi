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

/**
 * The Object class.
 */

#ifndef __TEMPI_OBJECT_H__
#define __TEMPI_OBJECT_H__

#include <string>

namespace tempi
{

/**
 * An Object is a generic Tempi entity that has a name and some documentation attached to it.
 */
class Object
{
    public:
        /**
         * Constructor.
         */
        Object(
            const char *name = "",
            const char *short_documentation = "",
            const char *long_documentation = "");
        /**
         * Returns the name of this Object.
         */
        std::string getName() const;
        /**
         * Returns the short documentation string of this Object.
         */
        std::string getShortDocumentation() const;
        /**
         * Returns the long documentation string of this Object.
         */
        std::string getLongDocumentation() const;
    protected:
        /**
         * Sets the name of this Object.
         */
        std::string setName(const char *name);
        /**
         * Sets the short documentation string of this Object.
         */
        std::string setShortDocumentation(const char *short_documentation);
        /**
         * Sets the long documentation string of this Object.
         */
        std::string setLongDocumentation(const char *long_documentation);
    private:
        std::string name_;
        std::string short_documentation_;
        std::string long_documentation_;
};

} // end of namespace

#endif // ifndef

