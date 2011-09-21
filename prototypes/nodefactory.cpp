// see http://stackoverflow.com/questions/2850213/dynamically-register-constructor-methods-in-an-abstractfactory-at-compile-time-us
#include <map>
#include <iostream>
#include <string>
#include <tr1/memory>

class Node; // forward declaration

class AbstractNodeType
{
    public:
        //virtual const std::string &getId() const = 0;
        virtual Node *create() = 0;
};
/**
 * An entry in the registry.
 */
template <class T>
class NodeType
{
    public:
        //virtual const std::string &getId() const
        //{
        //    return T::factory_key;
        //}
        virtual Node *create()
        {
            return new T();
        }
};

class NodeFactory
{
    public:
        bool registerType(const char *name, std::tr1::shared_ptr<AbstractNodeType> entry)
        {
            std::cout << "Register node type " << name << std::endl;
            entries_[std::string(name)] = entry;
            return true;
        }
        bool registerType(const char *name, AbstractNodeType *entry)
        {
            return registerType(name, std::tr1::shared_ptr<AbstractNodeType>(entry));
        }
        Node *create(const char *name)
        {
            // TODO: throw error if type not found
            return entries_[std::string(name)].get()->create();
        }
    private:
        std::map<std::string, std::tr1::shared_ptr<AbstractNodeType> > entries_;
};

class Node
{
    public:
        Node() {}
        virtual void say()
        {
            std::cout << "I am just a Node." << std::endl;
        }
};

class FooNode : public Node
{
    public:
        FooNode() :
            Node()
        {
            std::cout << "Created FooNode." << std::endl;
        }
        virtual void say()
        {
            std::cout << "I am a FooNode." << std::endl;    
        }
};

class BarNode : public Node
{
    public:
        BarNode() :
            Node()
        {
            std::cout << "Created BarNode." << std::endl;
        }
        virtual void say()
        {
            std::cout << "I am a BarNode." << std::endl;    
        }
};

int main(int argc, char **argv)
{
    NodeFactory factory;
    factory.registerType("foo", (AbstractNodeType*) new NodeType<FooNode>());
    factory.registerType("bar", (AbstractNodeType*) new NodeType<BarNode>());

    Node *foo = factory.create("foo");
    foo->say();

    Node *bar = factory.create("bar");
    bar->say();

    delete foo;
    delete bar;
}

