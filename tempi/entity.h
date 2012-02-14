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
 * The Entity class.
 */

#ifndef __TEMPI_ENTITY_H__
#define __TEMPI_ENTITY_H__

#include <string>
#include "tempi/namedobjectmap.h"
#include "tempi/documented.h"
#include "tempi/attribute.h"
#include "tempi/nodesignal.h"

namespace tempi
{

// TODO: add methods
/**
 * An Entity is a generic object type that has a name and some documentation attached to it, but also dynamic sets of signals and attributes.
 */
class Entity : public Documented
{
    public:
        /**
         * Constructor.
         */
        Entity(
            const char *name = "",
            const char *short_documentation = "",
            const char *long_documentation = "");
        /**
         * Retrieves a Signal.
         */
        NodeSignal* getSignal(const char *name) const
            throw(BadIndexException);
        /**
         * Retrieves an Attribute.
         */
        Attribute* getAttribute(const char *name) const
            throw(BadIndexException);

        // TODO: protected?
        virtual void onAttributeChanged(const char *name, const Message &value)
        {}
        /**
         * Returns the value of a given named attribute of this Entity.
         */
        const Message &getAttributeValue(const char *name) const
            throw(BadIndexException);
        /**
         * Return the list of all the attributes names for this Entity.
         * Note that some attributes can appear/disappear at run time.
         */
        std::vector<std::string> listAttributes() const;
        /**
         * Return the list of all the signals names for this Entity.
         * Note that some signals can appear/disappear at run time.
         */
        std::vector<std::string> listSignals() const;

        /**
         * Returns the value of a given named attribute of this Entity.
         */
        //const Message &getAttributeValue(const char *name) const
        //    throw(BadIndexException);
        /**
         * Checks if this Entity has a given attribute.
         * @return True if this Entity has such an attribute.
         */
        bool hasAttribute(const char *name) const
            throw(BadIndexException);
        /**
         * Checks if this Entity has a given signal.
         * @return True if this Entity has such an signal.
         */
        bool hasSignal(const char *signal_name) const
            throw(BadIndexException);
        /**
         * Sets a attribute value.
         * You can also do this by sending a message in the form s:"set" s:name ...
         * WARNING: if the value has not changed, it won't call onAttributeChanged.
         * TODO: rename to setAttributeValue and delete this one.
         */
        void setAttribute(const char *name, const Message &value)
            throw(BadIndexException, BadArgumentTypeException);

        void setAttributeValue(const char *name, const Message &value)
            throw(BadIndexException, BadArgumentTypeException)
        {
            setAttribute(name, value);
        }
//     protected:
        /**
         * Adds an attribute to this Entity.
         */
        void newAttribute(Attribute* attribute)
            throw(BadIndexException);
        /**
         * Adds a signal to this Entity.
         */
        void addSignal(NodeSignal* signal)
            throw(BadIndexException);
    private:
        NamedObjectMap<Attribute> attributes_;
        NamedObjectMap<NodeSignal> signals_;
};

} // end of namespace

#endif // ifndef

