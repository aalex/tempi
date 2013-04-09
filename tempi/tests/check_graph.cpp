#include <iostream>
#include "tempi/graph.h"
#include "tempi/log.h"
#include "tempi/node.h"
#include "tempi/nodefactory.h"

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
            this->addInlet("in", "");
            this->addOutlet("out", "");
        }
    private:
        virtual void processMessage(const char *inlet, const Message &message)
        {
            if (VERBOSE)
                std::cout << "DummyNode::" << __FUNCTION__ << ": " << message << std::endl;
            Message out = message;
        this->output("out", out);
    }
};

static bool fail(const char *message)
{
    std::cout << "FAIL: " << message << std::endl;
    return false;
}

bool check_node_factory()
{
    NodeFactory::ptr factory(new NodeFactory);
    factory->registerTypeT<DummyNode>("dummy");

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

    if (! graph.connect("dummy0", "out", "dummy1", "in"))
        return fail("Failed to connect dummy0 to dummy1");
    if (! graph.connect("dummy0", "out", "dummy2", "in"))
        return fail("Failed to connect dummy0 to dummy2");
    if (! graph.connect("dummy2", "out", "dummy3", "in"))
        return fail("Failed to connect dummy2 to dummy3");
    if (! graph.connect("dummy2", "out", "dummy4", "in"))
        return fail("Failed to connect dummy2 to dummy4");

    for (int i = 0; i < 10; ++i)
        graph.tick();

    //     if (graph.getAllConnections().size() != 4)
    //     {
    //         std::cout << "Expected 4 connections, got " << graph.getAllConnections().size() << std::endl;
    //         return false;
    //     }

    Message message = Message("fis", 3.14159f, 2, "hello");
    graph.message("dummy0", "in", message);
    return true;
}

bool check_disconnect()
{
    NodeFactory::ptr factory(new NodeFactory);
    factory->registerTypeT<DummyNode>("dummy");

    Graph graph(factory);
    if (! graph.addNode("dummy", "dummy0"))
        return fail("Failed to add dummy0");
    if (! graph.addNode("dummy", "dummy1"))
        return fail("Failed to add dummy1");
    if (! graph.addNode("dummy", "dummy2"))
        return fail("Failed to add dummy2");

    if (! graph.connect("dummy0", "out", "dummy1", "in"))
        return fail("Failed to connect dummy0 to dummy1");
    if (graph.getAllConnections().size() != 1)
    {
        std::ostringstream os;
        os << "Step 1: Expected " << 1 << " connection(s), got " << graph.getAllConnections().size();
        return fail(os.str().c_str());
    }

    if (! graph.connect("dummy0", "out", "dummy2", "in"))
        return fail("Failed to connect dummy0 to dummy2");
    if (graph.getAllConnections().size() != 2)
    {
        std::ostringstream os;
        os << "Step 2: Expected " << 2 << " connection(s), got " << graph.getAllConnections().size();
        return fail(os.str().c_str());
    }
    Logger& logger = Logger::getInstance();
    logger.setLevel(CRITICAL); // make the following ERROR quiet
    if (graph.connect("dummy0", "out", "dummy2", "in"))
        return fail("Should not be able to connect twice dummy0 to dummy2");
    logger.setLevel(NOTICE); // back to normal

    if (! graph.disconnect("dummy0", "out", "dummy2", "in"))
        return fail("Could not dicconnect dummy0 from dummy2");

    if (graph.getAllConnections().size() != 1)
    {
        std::ostringstream os;
        os << "Step 3: Expected " << 1 << " connection(s), got " << graph.getAllConnections().size();
        return fail(os.str().c_str());
    }

    for (int i = 0; i < 10; ++i)
        graph.tick();

    //     if (graph.getAllConnections().size() != 4)
    //     {
    //         std::cout << "Expected 4 connections, got " << graph.getAllConnections().size() << std::endl;
    //         return false;
    //     }

    Message message = Message("fis", 3.14159f, 2, "hello");
    graph.message("dummy0", "in", message);
    return true;
}

int main(int argc, char *argv[])
{
    if (! check_node_factory())
        return 1;
    if (! check_disconnect())
        return 1;
    return 0;
}

