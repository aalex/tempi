#include <iostream>
#include "tempi/tempi.h"

using namespace tempi;
static const bool VERBOSE = true;

/**
 * Dummy node.
 */
class DummyNode: public Node
{
    public:
        DummyNode() :
            Node()
        {
            addInlet();
            addOutlet();
        }
    private:
        virtual void processMessage(unsigned int inlet, const Message &message)
        {
            getOutlet(0)->trigger(message);
            if (VERBOSE)
                std::cout << "DummyNode::" << __FUNCTION__ << ": " << message << std::endl;
        }
};

static Node::ptr makeDummyNode()
{
    return Node::ptr(new DummyNode);
}

bool check_graph()
{
    Graph graph;

    graph.addNode("source0", makeDummyNode());
    graph.addNode("filter0", makeDummyNode());
    graph.addNode("sink0", makeDummyNode());
    graph.addNode("sink1", makeDummyNode());

    graph.connect("source0", 0, "filter0", 0);
    graph.connect("filter0", 0, "sink0", 0);
    graph.connect("filter0", 0, "sink1", 0);

    for (int i = 0; i < 10; ++i)
        graph.tick();

    Message message;
    message.appendFloat(3.14159f);
    message.appendInt(2);
    message.appendString("hello");
    graph.message("source0", 0, message);
    return true;
}

static bool fail(const char *message)
{
    std::cout << message << std::endl;
    return false;
}

bool check_node_factory()
{
    NodeFactory::ptr factory(new NodeFactory);
    factory->registerType("dummy", (AbstractNodeType *) new NodeType<DummyNode>);

    Graph graph(factory);
    // --------------------------------
    std::cout << *factory << std::endl;
    if (! graph.addNode("dummy0", "dummy"))
        return fail("Failed to add dummy0");
    else
        std::cout << "created dummy0 ok" << std::endl;

    // --------------------------------
    std::cout << *factory << std::endl;
    if (! graph.addNode("dummy1", "dummy"))
        return fail("Failed to add dummy1");
    else
        std::cout << "created dummy1 ok" << std::endl;

    // --------------------------------
    std::cout << *factory << std::endl;
    if (! graph.addNode("dummy2", "dummy"))
        return fail("Failed to add dummy2");
    else
        std::cout << "created dummy2 ok" << std::endl;

    // --------------------------------
    std::cout << *factory << std::endl;
    if (! graph.addNode("dummy3", "dummy"))
        return fail("Failed to add dummy3");
    else
        std::cout << "created dummy3 ok" << std::endl;

    // --------------------------------
    if (! graph.addNode("dummy4", "dummy"))
        return fail("Failed to add dummy4");

    if (! graph.connect("dummy0", 0, "dummy1", 0))
        return fail("Failed to connect dummy0 to dummy1");
    if (! graph.connect("dummy0", 0, "dummy2", 0))
        return fail("Failed to connect dummy0 to dummy2");
    if (! graph.connect("dummy2", 0, "dummy3", 0))
        return fail("Failed to connect dummy2 to dummy3");
    if (! graph.connect("dummy2", 0, "dummy4", 0))
        return fail("Failed to connect dummy2 to dummy4");

    for (int i = 0; i < 10; ++i)
        graph.tick();

    Message message;
    message.appendFloat(3.14159f);
    message.appendInt(2);
    message.appendString("hello");
    graph.message("dummy0", 0, message);

    return true;
}

int main(int argc, char *argv[])
{
    if (! check_graph())
        return 1;
//FIXME TODO:
//    if (! check_node_factory())
//        return 1;
    return 0;
}

