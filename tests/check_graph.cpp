#include <iostream>
#include "tempi/tempi.h"

using namespace tempi;
static const bool VERBOSE = false;

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

    graph.addNode(makeDummyNode(), "dummy", "source0");
    graph.addNode(makeDummyNode(), "dummy", "filter0");
    graph.addNode(makeDummyNode(), "dummy", "sink0");
    graph.addNode(makeDummyNode(), "dummy", "sink1");

    graph.connect("source0", 0, "filter0", 0);
    graph.connect("filter0", 0, "sink0", 0);
    graph.connect("filter0", 0, "sink1", 0);

    for (int i = 0; i < 10; ++i)
        graph.tick();

    Message message = Message("fis", 3.14159f, 2, "hello");
    graph.message("source0", 0, message);

    if (graph.getAllConnections().size() != 3)
    {
        std::cout << "Expected 3 connections, got " << graph.getAllConnections().size() << std::endl;
        return false;
    }
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
    AbstractNodeType::ptr dummy((AbstractNodeType*) (new NodeType<DummyNode>));
    factory->registerType("dummy", dummy);

    Graph graph(factory);
    if (! graph.addNode("dummy", "dummy0"))
        return fail("Failed to add dummy0");
    if (! graph.addNode("dummy", "dummy1"))
        return fail("Failed to add dummy1");
    if (! graph.addNode("dummy", "dummy2"))
        return fail("Failed to add dummy2");
    if (! graph.addNode("dummy", "dummy3"))
        return fail("Failed to add dummy3");
    if (! graph.addNode("dummy", "dummy4"))
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

    Message message = Message("fis", 3.14159f, 2, "hello");
    graph.message("dummy0", 0, message);
    return true;
}

int main(int argc, char *argv[])
{
    if (! check_graph())
        return 1;
    if (! check_node_factory())
        return 1;
    return 0;
}

