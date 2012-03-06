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

#include "abstractfactory.h"
#include "instancecreator.h"
#include <iostream>
#include <sstream>

namespace tempi
{

bool AbstractFactory::registerTypePtr(const char *name, AbstractInstanceCreator::ptr entry)
    throw(BadInstanceNameException)
{
    if (hasType(name))
    {
        std::ostringstream os;
        os << "AbstractFactory::" << __FUNCTION__ << "Already got an entry named " << name;
        throw (BadInstanceNameException(os.str().c_str()));
        return false;
    }
    if (entry.get() == 0)
    {
        std::ostringstream os;
        os << "AbstractFactory::" << __FUNCTION__ << ": " << name << " is a " << "null pointer!";
        throw (BadInstanceNameException(os.str().c_str()));
        return false;
    }
    entries_[std::string(name)] = entry;
    return true;
}

Instance::ptr AbstractFactory::create(const char *name)
    throw(BadInstanceNameException)
{
    if (! hasType(name))
    {
        std::ostringstream os;
        os << "AbstractFactory::" << __FUNCTION__ << ": No such node type: " << name;
        throw BadInstanceNameException(os.str().c_str());
    }
    std::string nameString(name);
    AbstractInstanceCreator *creator = entries_[nameString].get();
    Instance::ptr instance(creator->create());
    instance->setTypeName(name);
    return instance;
}

bool AbstractFactory::hasType(const char *name)
{
    bool ret = false;
    std::string nameStr(name);
    std::map<std::string, AbstractInstanceCreator::ptr>::iterator iter;
    for (iter = entries_.begin(); iter != entries_.end(); ++iter)
    {
        if ((*iter).first == name)
        {
            ret = true;
            break;
        }
    }
    return ret;
}

std::ostream &operator<<(std::ostream &os, const AbstractFactory &abstractFactory)
{
    os << "AbstractFactory:" << std::endl;
    std::map<std::string, AbstractInstanceCreator::ptr> entries = abstractFactory.getEntries();
    std::map<std::string, AbstractInstanceCreator::ptr>::iterator iter;
    for (iter = entries.begin(); iter != entries.end(); ++iter)
    {
        os << " * " << (*iter).first << ": " << (*iter).second.get() << std::endl;
    }
    os << "----------------" << std::endl;
    return os;
}

} // end of namespace

