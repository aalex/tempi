#include "tempi/tempi.h"
#include <iostream>

using namespace tempi;
static const bool VERBOSE = false;

class FooNode: public Node
{
    public:
        FooNode() :
            Node()
        {
            if (VERBOSE)
                std::cout << "Create a FooNode" << std::endl;
            addInlet();
            addOutlet();
        }
    private:
        virtual void processMessage(unsigned int inlet, const Message &message) {}
};

class BarNode: public Node
{
    public:
        BarNode() :
            Node()
        {
            addInlet();
            addOutlet();
            if (VERBOSE)
                std::cout << "Create a BarNode" << std::endl;
        }
    private:
        virtual void processMessage(unsigned int inlet, const Message &message) {}
};

bool check_nodefactory()
{
    if (VERBOSE)
        std::cout << __FUNCTION__ << std::endl;
    NodeFactory factory;
    AbstractNodeType *tmp = (AbstractNodeType*) new NodeType<FooNode>();
    AbstractNodeType::ptr fooType(tmp);
    if (fooType.get() == 0)
        std::cout << __FUNCTION__ << ": FooType is a null pointer!!!!!!" << std::endl;
    if (! factory.registerType("foo", fooType))
    {
        std::cout << "Could not register type FooNode" << std::endl;
        return false;
    }
    AbstractNodeType::ptr barType((AbstractNodeType*) new NodeType<BarNode>);
    if (! factory.registerType("bar",  barType))
    {
        std::cout << "Could not register type BarNode" << std::endl;
        return false;
    }

    //std::cout << "Registered FooNode and BarNode types." << std::endl;
    //std::cout << factory << std::endl;
    try
    {
        Node::ptr foo = factory.create("egg");
        return false;
    }
    catch (const BadNodeTypeException &e)
    {
        //std::cout << e.what() << std::endl;
        //std::cout << "Good it could not create a EggNode" << std::endl;
    }
    Node::ptr foo = factory.create("foo");
    Node::ptr bar = factory.create("bar");

    Message message;
    foo.get()->getInlet(0)->trigger(message);
    bar.get()->getInlet(0)->trigger(message);
    return true;
}

bool check_many_instances()
{
    if (VERBOSE)
        std::cout << __FUNCTION__ << std::endl;
    NodeFactory factory;
    if (factory.registerTypeT<FooNode>("foo").get() == 0)
    {
        std::cout << "Could not register type FooNode" << std::endl;
        return false;
    }
    Node::ptr foo0 = factory.create("foo");
    Node::ptr foo1 = factory.create("foo");
    Node::ptr foo2 = factory.create("foo");
    if (foo0.get() == 0 || foo0.get() == 0 || foo0.get() == 0)
    {
        std::cout << __FUNCTION__ << ": invalid pointer" << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char **argv)
{
    if (! check_nodefactory())
        return 1;
    if (! check_many_instances())
        return 1;
    return 0;
}

