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
 * Some helper classes for the AbstractFactory.
 */
#ifndef __TEMPI_INSTANCE_CREATOR_H__
#define __TEMPI_INSTANCE_CREATOR_H__

#include <tr1/memory>

namespace tempi
{

class Instance
{
    public:
        typedef std::tr1::shared_ptr<Instance> ptr;
        void setTypeName(const char *name)
        {
            typeName_ = name;
        }
        const std::string &getTypeName() const
        {
            return typeName_;
        }
    private:
        std::string typeName_;
};

// std::ostream &operator<<(std::ostream &os, const Instance &instance)
// {
//     os << instance.getTypeName() << " " << std::endl;
//     return os;
// }
/**
 * An entry in the AbstractFactory.
 */
class AbstractInstanceCreator
{
    public:
        typedef std::tr1::shared_ptr<AbstractInstanceCreator> ptr;
        virtual Instance *create() = 0;
};

/**
 * An entry in the NodeFactory, specific to a single Node type.
 */
template <class T>
class InstanceCreator
{
    public:
        virtual Instance *create()
        {
            return new T();
        }
};

} // end of namespace

#endif // ifndef

