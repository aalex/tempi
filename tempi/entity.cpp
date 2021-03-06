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

#include "tempi/entity.h"
#include "tempi/log.h"
#include "tempi/utils.h"

namespace tempi {

Entity::Entity(
    const char *name,
    const char *short_documentation,
    const char *long_documentation
    ) :
    Documented(name, short_documentation, long_documentation)
{
}

Attribute* Entity::getAttribute(const char *name) const
    throw(BadIndexException)
{
    return attributes_.get(name).get();
}

EntitySignal* Entity::getSignal(const char *name) const
    throw(BadIndexException)
{
    return signals_.get(name).get();
}

EntityMethod* Entity::getMethod(const char *name) const
    throw(BadIndexException)
{
    return methods_.get(name).get();
}

const Message &Entity::getAttributeValue(const char *name) const
    throw(BadIndexException)
{
    return getAttribute(name)->getValue();
}


void Entity::setAttributeValue(const char *name, const Message &value)
    throw(BadIndexException, BadAtomTypeException)
{
    bool ok_to_change = false;
    Attribute* current = getAttribute(name); // might throw BadIndexException
    //std::cout << "Node::" << __FUNCTION__ << ": " << name << " = " << value << std::endl;
    if (current->isTypeStrict())
    {
        if (current->getValue().getTypes().compare(value.getTypes()) == 0)
        {
            ok_to_change = true;
        }
        else
        {
            Message casted_message;
            if (utils::tryAutoCast(value, casted_message, current->getValue().getTypes()))
            {
                if (Logger::isEnabledFor(DEBUG))
                {
                    std::ostringstream os;
                    os << "Entity." << __FUNCTION__ << ": (" << this->getName() << ") \"" << name << "\"=" << casted_message;
                    Logger::log(DEBUG, os);
                }
                current->setValue(casted_message);
                return;
            }
            // else
            std::ostringstream os;
            os << "Entity::" << __FUNCTION__ << ": Attribute " << name << ": Bad type " << value.getTypes() << " while expecting " << current->getValue().getTypes();
            throw (BadAtomTypeException(os.str().c_str()));
        }
    }
    else
        ok_to_change = true;
    // do it:
    if (ok_to_change)
    {
        // TODO: //if (isInitiated())
        if (Logger::isEnabledFor(DEBUG))
        {
            std::ostringstream os;
            os << "Entity." << __FUNCTION__ << ": (" << this->getName() << ") \"" << name << "\"=" << value;
            Logger::log(DEBUG, os);
        }
        ok_to_change = onAttributeChanged(name, value);
        if (ok_to_change)
            current->setValue(value);
    }
}

bool Entity::hasAttribute(const char *name) const
    throw (BadIndexException)
{
    return attributes_.has(name);
}

void Entity::addAttribute(Attribute::ptr attribute)
    throw(BadIndexException)
{
    attributes_.add(attribute);
    // deletes the NamedObject* if name is already taken
}

std::vector<std::string> Entity::listAttributes() const
{
    return attributes_.listNames();
}

bool Entity::hasSignal(const char *name) const
    throw(BadIndexException)
{
    return signals_.has(name);
}

bool Entity::hasMethod(const char *name) const
    throw(BadIndexException)
{
    return methods_.has(name);
}

void Entity::addSignal(EntitySignal::ptr signal) // TODO: rename to newSignal?
    throw(BadIndexException)
{
    if (signal.get() == 0)
    {
        throw(BadIndexException("Null pointer!")); // FIXME: throw another exception type
    }
    signals_.add(signal);
    // deletes the NamedObject* if name is already taken
}

void Entity::addMethod(EntityMethod::ptr method) // TODO: rename to newMethod?
    throw(BadIndexException)
{
    if (method.get() == 0)
    {
        throw(BadIndexException("Null pointer!")); // FIXME: throw another exception type
    }
    methods_.add(method);
    // deletes the NamedObject* if name is already taken
}

std::vector<std::string> Entity::listSignals() const
{
    return signals_.listNames();
}

std::vector<std::string> Entity::listMethods() const
{
    return methods_.listNames();
}

bool Entity::callMethod(const char *method_name, const Message &arguments, Message & return_value)
    throw(BadIndexException)
{
    return methods_.get(method_name)->call(arguments, return_value);
}

} // end of namespace

