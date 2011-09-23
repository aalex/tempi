#include "tempi/tempi.h"
#include <iostream>

using namespace tempi;

class FooNode: public Node
{
    public:
        FooNode() :
            Node()
        {
            std::cout << "Create a FooNode" << std::endl;
            addInlet();
            addOutlet();
        }
    private:
        virtual void processMessage(const Message &message) {}
};

class BarNode: public Node
{
    public:
        BarNode() :
            Node()
        {
            addInlet();
            addOutlet();
            std::cout << "Create a BarNode" << std::endl;
        }
    private:
        virtual void processMessage(const Message &message) {}
};

bool check_nodefactory()
{
    NodeFactory factory;
    if (! factory.registerType("foo", (AbstractNodeType*) (new NodeType<FooNode>())))
    {
        std::cout << "Could not register type FooNode" << std::endl;
        return false;
    }
    if (! factory.registerType("bar", (AbstractNodeType*)(new NodeType<BarNode>())))
    {
        std::cout << "Could not register type BarNode" << std::endl;
        return false;
    }

    //std::cout << "Registered FooNode and BarNode types." << std::endl;
    //std::cout << factory << std::endl;
    try
    {
        std::tr1::shared_ptr<Node> foo = factory.create("egg");
        return false;
    }
    catch (const BadNodeTypeException &e)
    {
        //std::cout << e.what() << std::endl;
        //std::cout << "Good it could not create a EggNode" << std::endl;
    }
    std::tr1::shared_ptr<Node> foo = factory.create("foo");
    std::tr1::shared_ptr<Node> bar = factory.create("bar");

    Message message;
    foo.get()->getInlet(0)->trigger(message);
    bar.get()->getInlet(0)->trigger(message);

    return true;
}

int main(int argc, char **argv)
{
    if (! check_nodefactory())
        return 1;
    return 0;
}

