#include "tempi/tempi.h"
#include "tempi/graph.h"
#include "tempi/internals.h"
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
    internals::loadInternals(factory);
    if (factory.registerTypeT<FooNode>("foo").get() == 0)
    {
        std::cout << "Could not register type FooNode" << std::endl;
        return false;
    }
    Node::ptr foo0 = factory.create("foo");
    Node::ptr foo1 = factory.create("foo");
    Node::ptr foo2 = factory.create("base.nop");
    if (foo0.get() == 0 || foo1.get() == 0 || foo2.get() == 0)
    {
        std::cout << __FUNCTION__ << ": invalid pointer" << std::endl;
        return false;
    }
    return true;
}

bool check_print()
{
    if (VERBOSE)
        std::cout << __FUNCTION__ << std::endl;
    NodeFactory::ptr factory(new NodeFactory);
    internals::loadInternals(*(factory.get()));
    Node::ptr nop0 = factory->create("base.nop");
    Node::ptr nop1 = factory->create("base.nop");
    Node::ptr print0 = factory->create("base.print");
    Node::ptr sampler0 = factory->create("sampler.sampler");
    Node::ptr print1 = factory->create("base.print");

    if (nop0.get() == 0 || print0.get() == 0 || sampler0.get() == 0)
    {
        std::cout << __FUNCTION__ << ": invalid pointer" << std::endl;
        return false;
    }
    Graph graph(factory);
    graph.addNode("base.nop", "nop0");
    graph.addNode("base.nop", "nop1");
    graph.addNode("base.print", "print0");
    graph.addNode("sampler.sampler", "sampler0");
    graph.addNode("base.print", "print1");

    graph.connect("nop0", 0, "nop1", 0);
    graph.connect("nop1", 0, "print0", 0);
    graph.connect("nop1", 0, "sampler0", 0);
    graph.connect("sampler0", 0, "print1", 0);

    // disable print
    Message disable_message = Message("ssb", "set", "enabled", false);
    bool quiet = ! VERBOSE;
    if (quiet)
    {
        graph.message("print0", 0, disable_message);
        graph.message("print1", 0, disable_message);
    }
    // change prefix
    //FIXME: both [print] objects have same value for prefix property.
    Message prefix0_message = Message("sss", "set", "prefix", "recording: ");
    graph.message("print0", 0, prefix0_message);
    Message prefix1_message = Message("sss", "set", "prefix", "playback: ");
    graph.message("print1", 0, prefix1_message);
    //std::cout << "sampler0 has n inlets: " << graph.getNode("sampler0")->getNumberOfInlets() << std::endl;
    // enables the sampler
    // FIXME: property inlet in hard-coded to 0 in Node
    Message playing = Message("ssb", "set", "playing", true);
    graph.message("sampler0", 0, playing);
    Message recording = Message("ssb", "set", "recording", true);
    graph.message("sampler0", 0, recording);
    // print something (or not is disabled)
    Message fis_message = Message("fis", 3.14159f, 2, "hello");
    graph.message("nop0", 0, fis_message);
    graph.tick();
    graph.message("nop0", 0, fis_message);
    graph.tick();

    recording.setBoolean(2, false);
    graph.message("sampler0", 0, recording);
    playing.setBoolean(2, false);
    graph.message("sampler0", 0, playing);
    playing.setBoolean(2, true);
    graph.message("sampler0", 0, playing);
    recording = Message("ssb", "set", "recording", true);
    graph.message("sampler0", 0, recording);

    for (int i = 0; i < 10; ++i)
    {
        graph.tick();
    }

    return true;
}

int main(int argc, char **argv)
{
    if (! check_nodefactory())
        return 1;
    if (! check_many_instances())
        return 1;
    if (! check_print())
        return 1;
    return 0;
}

