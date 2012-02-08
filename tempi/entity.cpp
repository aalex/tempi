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

#include "tempi/entity.h"

namespace tempi
{

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
    return dynamic_cast<Attribute*>(attributes_.get(name).get());
}

NodeSignal* Entity::getSignal(const char *name) const
    throw(BadIndexException)
{
    return dynamic_cast<NodeSignal*>(signals_.get(name).get());
}

const Message &Entity::getAttributeValue(const char *name) const
    throw(BadIndexException)
{
    return getAttribute(name)->getValue();
}

// TODO: rename to setAttributeValue
void Entity::setAttribute(const char *name, const Message &value)
    throw(BadIndexException, BadArgumentTypeException)
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
            std::ostringstream os;
            os << "Entity::" << __FUNCTION__ << ": Attribute " << name << ": Bad type " << value.getTypes() << " while expecting " << current->getValue().getTypes();
            throw (BadArgumentTypeException(os.str().c_str()));
        }
    }
    else
        ok_to_change = true;
    // do it:
    if (ok_to_change)
    {
        current->setValue(value);
        // TODO: //if (isInitiated())
        onAttributeChanged(name, value);
    }
}

bool Entity::hasAttribute(const char *name) const
    throw (BadIndexException)
{
    return attributes_.has(name);
}

void Entity::newAttribute(Attribute* attribute)
    throw(BadIndexException)
{
    attributes_.add(NamedObject::ptr(dynamic_cast<NamedObject*>(attribute)));
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

void Entity::addSignal(NodeSignal* signal) // TODO: rename to newSignal?
    throw(BadIndexException)
{
    if (signal == 0)
    {
        throw(BadIndexException("Null pointer!")); // FIXME: throw another exception type
    }
    signals_.add(NamedObject::ptr(dynamic_cast<NamedObject*>(signal)));
    // deletes the NamedObject* if name is already taken
}

std::vector<std::string> Entity::listSignals() const
{
    return signals_.listNames();
}

} // end of namespace

