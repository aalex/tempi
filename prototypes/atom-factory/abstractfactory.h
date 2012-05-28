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
 * The NodeFactory, NodeType and AbstractNodeType classes.
 */

#ifndef __TEMPI_ABSTRACT_FACTORY_H__
#define __TEMPI_ABSTRACT_FACTORY_H__

#include <map>
#include <string>
#include <sstream>
#include "instancecreator.h"
#include <tr1/memory>
#include <stdexcept>
#include <string>

namespace tempi
{

class BadInstanceNameException : public std::runtime_error
{
    public:
        BadInstanceNameException(const char *message) :
            std::runtime_error(message)
        {}
};

/**
 * An AbstractFactory stores creators for some types.
 * see http://stackoverflow.com/questions/2850213/dynamically-register-constructor-methods-in-an-abstractfactory-at-compile-time-us
 */
class AbstractFactory
{
    public:
        typedef std::tr1::shared_ptr<AbstractFactory> ptr;

        /**
         * Registers a type.
         */
        template <class T>
        bool registerType(const char *name)
            throw (BadInstanceNameException)
        {
            AbstractInstanceCreator *tmp = (AbstractInstanceCreator*) new InstanceCreator<T>();
            AbstractInstanceCreator::ptr ret(tmp);
            if (! registerTypePtr(name, ret))
            {
                std::ostringstream os;
                os << "Could not register type " << name << " ";
                os << "Name already taken";
                throw (BadInstanceNameException(os.str().c_str()));
            }
            return ret;
        }
        /**
         * Checks for a given node type.
         */
        bool hasType(const char *name);
        /**
         * Creates an Instance.
         * You are responsible for freeing the memory allocated to this pointer.
         * No memory is allocated if it throws an exception.
         */
        Instance* create(const char *name)
            throw(BadInstanceNameException);
        /**
         * Returns all entries in this AbstractFactory.
         */
        std::map<std::string, AbstractInstanceCreator::ptr> getEntries() const
        {
            return entries_;
        }
    private:
        std::map<std::string, AbstractInstanceCreator::ptr> entries_;
        bool registerTypePtr(const char *name, AbstractInstanceCreator::ptr entry)
            throw (BadInstanceNameException);
};

std::ostream &operator<<(std::ostream &os, const AbstractFactory &nodeFactory);

} // end of namespace

#endif // ifndef

