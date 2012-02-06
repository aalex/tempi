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
 * The Named class.
 */

#ifndef __TEMPI_NAMEDOBJECT_H__
#define __TEMPI_NAMEDOBJECT_H__

#include <string>
#include <tr1/memory>

namespace tempi
{

/**
 * An NamedObject is an object that has a name.
 */
class NamedObject
{
    public:
        /**
         * Shared pointer to a NamedObject.
         */
        typedef std::tr1::shared_ptr<NamedObject> ptr;
        /**
         * Constructor.
         */
        NamedObject(const char *name = "");
        /**
         * Returns the name of this Documented.
         */
        std::string getName() const;
//    protected:
        /**
         * Sets the name of this Documented.
         */
        std::string setName(const char *name);
        /**
         * A virtual destructor makes this class polymorphic.
         */
        virtual ~NamedObject() {}
    private:
        std::string name_;
};

} // end of namespace

#endif // ifndef

