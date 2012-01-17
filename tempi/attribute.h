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
 * @file
 * The Attribute class
 */

#ifndef __TEMPI_ATTRIBUTE_H__
#define __TEMPI_ATTRIBUTE_H__

#include <iostream>
#include <string>
#include "tempi/message.h"
#include "tempi/sharedptr.h"

namespace tempi {

/**
 * An Attribute is used to store info in a Node and to control its behaviour.
 */
class Attribute
{
    public:
        typedef std::tr1::shared_ptr<Attribute> ptr;
        Attribute(const char *name, const Message &value, const char *doc, bool type_strict=true);
        std::string getName() const;
        std::string getDocumentation() const;
        const Message &getValue();
        bool setValue(const Message &value);
        bool isTypeStrict() const;
    private:
        std::string name_;
        Message value_;
        std::string documentation_;
        bool type_strict_;
};

} // end of namespace

#endif // ifndef

