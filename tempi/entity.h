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
        typedef std::tr1::shared_ptr<Entity> ptr;
        /**
         * Constructor.
         */
        Entity(
            const char *name = "",
            const char *short_documentation = "",
            const char *long_documentation = "");
        
        /**
         * A virtual destructor makes this class polymorphic.
         */
        virtual ~Entity() {}
        /**
         * Retrieves a Signal.
         */
        NodeSignal* getSignal(const char *name) const
            throw(BadIndexException);
        /**
         * Retrieves a Method.
         */
        NodeSignal* getMethod(const char *name) const
            throw(BadIndexException);
        /**
         * Retrieves an Attribute.
         */
        Attribute* getAttribute(const char *name) const
            throw(BadIndexException);

        // TODO: protected?
        /**
         * Called just before changing the value of an Attribute.
         * Entities return true if it's ok to change its value.
         * They can still query the former/current value of that attribute.
         * Return false, and the value is not going to be changed.
         */
        virtual bool onAttributeChanged(const char *name, const Message &value)
        {
            return true;
        }
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
         * Return the list of all the methods names for this Entity.
         * Note that some methods can appear/disappear at run time.
         */
        std::vector<std::string> listMethods() const;

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
         * @return True if this Entity has such a signal.
         */
        bool hasSignal(const char *signal_name) const
            throw(BadIndexException);
        /**
         * Checks if this Entity has a given method.
         * @return True if this Entity has such a method.
         */
        bool hasMethod(const char *method_name) const
            throw(BadIndexException);
        /**
         * Sets a attribute value.
         * You can also do this by sending a message in the form s:"set" s:name ...
         * WARNING: if the value has not changed, it won't call onAttributeChanged.
         */
        void setAttributeValue(const char *name, const Message &value)
            throw(BadIndexException, BadAtomTypeException);
//     protected:
        /**
         * Adds an attribute to this Entity.
         */
        void addAttribute(Attribute::ptr attribute)
            throw(BadIndexException);
        /**
         * Adds a signal to this Entity.
         */
        void addSignal(NodeSignal::ptr signal)
            throw(BadIndexException);
        /**
         * Adds a method to this Entity.
         */
        void addMethod(NodeSignal::ptr method)
            throw(BadIndexException);
    private:
        NamedObjectMap<Attribute> attributes_;
        NamedObjectMap<NodeSignal> signals_;
        NamedObjectMap<NodeSignal> methods_;
};

} // end of namespace

#endif // ifndef

