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
 * The NamedObjectMap template class.
 */
#ifndef __TEMPI_NAMEDOBJECTMAP_H__
#define __TEMPI_NAMEDOBJECTMAP_H__

#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "tempi/namedobject.h"
#include "tempi/exceptions.h"
#include "tempi/sharedptr.h"

namespace tempi
{

/**
 * Map containing named NamedObject instances.
 * T must be a shared_ptr to a child of NamedObject.
 */
//template <typename T>
class NamedObjectMap
{
    public:
        typedef std::tr1::shared_ptr<NamedObjectMap> ptr;
        /**
         * Adds a named object to this map.
         */
        void add(NamedObject::ptr entity)
            throw(BadIndexException)
        {
            if (entity.get() == 0)
            {
                std::cerr << "Null pointer in " << "NamedObjectMap::" << __FUNCTION__ << std::endl; // FIXME: throw another exception type?
            }
            if (has(entity->getName().c_str()))
            {
                std::ostringstream os;
                os << "ERROR in NamedObjectMap::" << __FUNCTION__ << ": Already has entity named " << entity->getName();
                throw (BadIndexException(os.str().c_str()));
            }
            objects_[entity->getName()] = entity;
        }
        /**
         * Checks if this map has an named object named with a given name.
         */
        bool has(const char *name) const
            throw(BadIndexException)
        {
            if (name == 0)
                throwNullStringException(__FUNCTION__);
            return (objects_.find(std::string(name)) != objects_.end());
        }
        /**
         * Lists the names of all the object in this map.
         */
        std::vector<std::string> listNames() const
        {
            std::vector<std::string> ret;
            MapType::const_iterator iter;
            for (iter = objects_.begin(); iter != objects_.end(); ++iter)
                ret.push_back((*iter).first);
            return ret;
        }
        /**
         * Removes a named object with a given name from this map.
         */
        void remove(const char *name)
            throw(BadIndexException)
        {
            if (name == 0)
                throwNullStringException(__FUNCTION__);
            if (! has(name))
            {
                std::ostringstream os;
                os << "ERROR in NamedObjectMap::" << __FUNCTION__ << ": Map doesn't have an entity named " << name;
                throw (BadIndexException(os.str().c_str()));
            }
            MapType::iterator iter = objects_.find(std::string(name));
            objects_.erase(iter);
        }
        /**
         * Retrieves a named object in this map.
         */
        NamedObject::ptr get(const char *name) const
            throw(BadIndexException)
        {
            if (name == 0)
                throwNullStringException(__FUNCTION__);
            MapType::const_iterator iter =
                objects_.find(std::string(name));
            if (iter == objects_.end())
            {
                std::ostringstream os;
                os << "ERROR in NamedObjectMap::" << __FUNCTION__ << ": Map doesn't have an entity named " << name;
                throw (BadIndexException(os.str().c_str()));
            }
            return (*iter).second;
        }
    private:
        typedef std::map<std::string, NamedObject::ptr> MapType;
        void throwNullStringException(const char *method_name) const
            throw(BadIndexException)
        {
                std::ostringstream os;
                os << "ERROR in NamedObjectMap::" << method_name << ": Got a null string as a NamedObject name.";
                throw (BadIndexException(os.str().c_str()));
        }
        MapType objects_;
};

} // end of namespace

#endif // ifndef

