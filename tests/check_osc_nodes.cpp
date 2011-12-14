#include <iostream>
#include <string>
#include "tempi/message.h"
#include "tempi/graph.h"
#include "tempi/osc/oscsendernode.h"
#include "tempi/osc/oscreceiver.h"
#include "tempi/nodefactory.h"

using namespace tempi;

static const bool VERBOSE = false;

static bool fail(const char *message)
{
    std::cout << message << std::endl;
    return false;
}

static bool check_oscsender_node()
{
    NodeFactory::ptr factory(new NodeFactory);

    factory->registerTypeT<osc::OscSenderNode>("osc.send");

    Graph graph(factory);

    if (! graph.addNode("osc.send", "sender0"))
        return fail("Failed to add sender0");

    Node *sender0 = graph.getNode("sender0").get();
    if (sender0 == 0)
    {
        return fail("Failed to get sender0 node.");
    }
    try
    {
        Message host_port;
        host_port.appendString("localhost");
        host_port.appendInt(14444);
        if (VERBOSE)
        {
            std::cout << "Set property host_port to " << host_port << std::endl;
        }
        sender0->setProperty("host_port", host_port);
    }
    catch (const BadIndexException &e)
    {
        std::cout << e.what() << std::endl;
        return false;
    }
    catch (const BadArgumentTypeException &e)
    {
        std::cout << e.what() << std::endl;
        return false;
    }

    Message m = Message("sfsi", "/hello", 3.14159f, "spam", 2);
    if (! graph.message("sender0", 0, m))
        return fail("Failed to message sender0");
    if (VERBOSE)
        std::cout << "Sent " << m << std::endl;
    return true;
}

int main(int argc, char *argv[])
{
    if (! check_oscsender_node())
        return 1;
    return 0;
}

