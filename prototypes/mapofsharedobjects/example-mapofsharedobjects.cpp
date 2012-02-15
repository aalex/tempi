#include <string>
#include <tr1/memory>
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

class BadIndexException : public std::runtime_error
{
    public:
        BadIndexException(const char *error_message) :
            std::runtime_error(error_message)
        {}
};

class NamedObject
{
    public:
        typedef std::tr1::shared_ptr<NamedObject> ptr;
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

template <typename T>
class NamedObjectMap
{
    public:
        typedef std::tr1::shared_ptr<T> T_ptr;
        typedef std::map<std::string, T_ptr> MapType;
        typedef typename MapType::const_iterator ConstIter;
        typedef typename MapType::iterator Iter;
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
            ConstIter iter;
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
            Iter iter = objects_.find(std::string(name));
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
            ConstIter iter = objects_.find(std::string(name));
            if (iter == objects_.end())
            {
                std::ostringstream os;
                os << "ERROR in NamedObjectMap::" << __FUNCTION__ << ": Map doesn't have an entity named \"" << name << "\".";
                os << " Names are: ";
                ConstIter iter2;
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

class IntermediateClass : public NamedObject
{
    public:
        typedef std::tr1::shared_ptr<IntermediateClass> ptr;
        IntermediateClass(const char *name) :
            NamedObject(name)
        {}
        virtual ~IntermediateClass()
        {}
};

class ExampleNamedObject : public IntermediateClass
{
    public:
        ExampleNamedObject(const char *name) :
            IntermediateClass(name)
        {}
        int getX() const
        {
            return x_;
        }
        void setX(int x)
        {
            x_ = x;
        }
    private:
        int x_; // extra attribute
};

class ExampleApp
{
    public:
        ExampleApp()
        {}
        void addOne(IntermediateClass::ptr obj)
        {
            objects_.add(obj);
        }
        void print() const
        {
            std::vector<std::string> names = objects_.listNames();
            std::vector<std::string>::const_iterator iter;
            for (iter = names.begin(); iter != names.end(); ++iter)
                std::cout << (*iter) << std::endl;
        }
    private:
        NamedObjectMap<IntermediateClass> objects_;
};

int main(int argc, char *argv[])
{
    ExampleApp app;
    app.addOne(IntermediateClass::ptr(new ExampleNamedObject("a")));
    app.addOne(IntermediateClass::ptr(new ExampleNamedObject("b")));
    app.addOne(IntermediateClass::ptr(new ExampleNamedObject("c")));
    app.addOne(IntermediateClass::ptr(new ExampleNamedObject("d")));
    app.print();
    return 0;
}

