#ifndef __ATOM_FACTORY_H__
#define __ATOM_FACTORY_H__

#include <typeinfo>
#include <map>
#include <sstream>
#include <tr1/memory>
#include "tempi/exceptions.h"

class BadAtomTypeException : public tempi::BaseException
{
    public:
        BadAtomTypeException(const char *error_message) :
            tempi::BaseException(error_message)
        {}
};

class AbstractAtomType
{
    public:
        typedef std::tr1::shared_ptr<AbstractAtomType> ptr;
        virtual Atom *create() = 0;
        virtual const std::type_info& getType() = 0;
};
template <typename T, class U>
class AtomType
{
    public:
        virtual Atom *create()
        {
            return new U();
        }
        virtual const std::type_info& getType()
        {
            return typeid(T);
        }
};

class AtomFactory
{
    public:
        typedef std::tr1::shared_ptr<AtomFactory> ptr;
        template <typename T>
        Atom::ptr create(T &value) throw(BadAtomTypeException)
        {
            const std::type_info& value_type = typeid(value);
            if (! this->hasType(value_type))
            {
                std::ostringstream os;
                os << "AtomFactory::" << __FUNCTION__ << ": No such atom type: " << value_type;
                throw BadAtomTypeException(os.str().c_str());
            }
            AbstractAtomType::ptr atomType = getTypeByType(value_type);
            Atom::ptr atom(atomType->create());
            return atom;
        }
        template <typename T, class U>
        AbstractAtomType::ptr registerType(const char name)
        {
            AbstractAtomType *tmp = (AbstractAtomType*) new AtomType<T, U>();
            AbstractAtomType::ptr ret(tmp);

            if (this->hasTypeName(name))
            {
                std::ostringstream os;
                os << "AtomFactory::" << __FUNCTION__ << "Already got an entry named " << name;
                //Logger::log(ERROR, os.str().c_str());
                std::cerr << os.str() << std::endl;
                //return false;
            }
            else
                entries_[name] = ret;
            return ret;
        }
        /**
         * Checks for a given atom type name.
         */
        bool hasTypeName(const char name)
        {
            bool ret = false;
            std::map<char, AbstractAtomType::ptr>::iterator iter;
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
        /**
         * Checks for a given atom value type.
         */
        bool hasType(const std::type_info &type)
        {
            bool ret = false;
            std::map<char, AbstractAtomType::ptr>::iterator iter;
            for (iter = entries_.begin(); iter != entries_.end(); ++iter)
            {
                if (((*iter).second)->getType() == type)
                {
                    ret = true;
                    break;
                }
            }
            return ret;
        }

        Atom::ptr create(const char name) throw(BadAtomTypeException)
        {
            if (! this->hasTypeName(name))
            {
                std::ostringstream os;
                os << "AtomFactory::" << __FUNCTION__ << ": No such atom type: " << name;
                throw BadAtomTypeException(os.str().c_str());
            }
            AbstractAtomType *atomType = entries_[name].get();

            Atom::ptr atom(atomType->create());
            return atom;
        }
    private:
        std::map<char, AbstractAtomType::ptr> entries_;
        // must have it or it might crash
        AbstractAtomType::ptr getTypeByType(const std::type_info &type)
        {
            AbstractAtomType::ptr ret;
            std::map<char, AbstractAtomType::ptr>::iterator iter;
            for (iter = entries_.begin(); iter != entries_.end(); ++iter)
            {
                if (((*iter).second)->getType() == type)
                {
                    ret = (*iter).second;
                }
            }
            return ret;
        }
};

#endif

