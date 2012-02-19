#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <string>
#include <tr1/memory>
#include <stdexcept>

class BaseException : public std::runtime_error
{
    public:
        BaseException(const char *error_message) :
            std::runtime_error(error_message)
        {}
        BaseException() :
            std::runtime_error("")
        {}
};

class BadIndexException : public BaseException
{
    public:
        BadIndexException(const char *error_message) :
            BaseException(error_message)
        {}
};

class NamedObject
{
    public:
        NamedObject(const char *name = "") :
            name_(name)
        {}
        std::string getName() const
        {
            return name_;
        }
        virtual ~NamedObject() {}
    private:
        std::string name_;
};

/**
 * Map containing named instances of children of NamedObject.
 * T must be a child of NamedObject.
 */
template <typename T>
class NamedObjectMap
{
    public:
        typedef std::tr1::shared_ptr<T> T_ptr;
        typedef std::map<std::string, T_ptr> MapType;
        /**
         * Adds a named object to this map.
         */
        void add(T_ptr entity)
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
        T_ptr get(const char *name) const
            throw(BadIndexException)
        {
            if (name == 0)
                throwNullStringException(__FUNCTION__);
            MapType::const_iterator iter =
                objects_.find(std::string(name));
            if (iter == objects_.end())
            {
                std::ostringstream os;
                os << "ERROR in NamedObjectMap::" << __FUNCTION__ << ": Map doesn't have an entity named \"" << name << "\".";
                os << " Names are: ";
                MapType::const_iterator iter2;
                for (iter2 = objects_.begin(); iter2 != objects_.end(); ++iter2)
                {
                    if (iter2 != objects_.begin())
                        os << " ";
                    os << (*iter2).first;
                }
                throw (BadIndexException(os.str().c_str()));
            }
            return (*iter).second;
        }
    private:
        void throwNullStringException(const char *method_name) const
            throw(BadIndexException)
        {
                std::ostringstream os;
                os << "ERROR in NamedObjectMap::" << method_name << ": Got a null string as a NamedObject name.";
                throw (BadIndexException(os.str().c_str()));
        }
        MapType objects_;
};

class Foo : public NamedObject
{
    public:
        Foo(const char *name) :
            NamedObject(name)
        {}
};

class Bar : public NamedObject
{
    public:
        Bar(const char *name) :
            NamedObject(name)
        {}
};

int main(int argc, char *argv[])
{
    NamedObjectMap<Foo> foos;
    NamedObjectMap<Bar> bars;
    foos.add(new Foo("foo0"));
    foos.add(new Foo("foo1"));
    bars.add(new Bar("bar0"));
    bars.add(new Bar("bar1"));
    std::vector<std::string> names = foos.listNames();
    std::cout << "Foos:" << std::endl;
    std::vector<std::string>::const_iterator iter;
    for (iter = foos.begin(); iter != foos.end(); ++foo)
    {
        std::cout << (*iter) << std::endl;
    }
    return 0;
}

