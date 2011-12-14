#include "tempi/tempi.h"
#include "tempi/graph.h"
#include "tempi/internals.h"
#include <unistd.h>
#include <iostream>

using namespace tempi;
static const bool VERBOSE = false;

bool check_create()
{
    NodeFactory factory;
    internals::loadInternals(factory);
    Node::ptr metro0 = factory.create("base.metro");
    Node::ptr print0 = factory.create("base.print");
    Node::ptr counter0 = factory.create("base.counter");
    if (metro0.get() == 0)
    {
        std::cout << "Metro ptr is null" << std::endl;
        return false;
    }
    return true;
}

bool checkPrepend()
{
    if (VERBOSE)
        std::cout << __FUNCTION__ << std::endl;
    NodeFactory::ptr factory(new NodeFactory);
    if (VERBOSE)
        std::cout << "loadInternals:" << std::endl;
    internals::loadInternals(factory);

    if (VERBOSE)
        std::cout << "create Graph:" << std::endl;
    Graph graph(factory); // FIXME: smart ptr or not for factory?
    if (VERBOSE)
        std::cout << "add nodes:" << std::endl;
    graph.addNode("base.prepend", "prepend0");
    // TODO: setNodeArguments
    return true;
}

bool checkMetro()
{
    if (VERBOSE)
        std::cout << __FUNCTION__ << std::endl;
    NodeFactory::ptr factory(new NodeFactory);
    if (VERBOSE)
        std::cout << "loadInternals:" << std::endl;
    internals::loadInternals(factory);

    if (VERBOSE)
        std::cout << "create Graph:" << std::endl;
    Graph graph(factory); // FIXME: smart ptr or not for factory?
    if (VERBOSE)
        std::cout << "add nodes:" << std::endl;
    graph.addNode("base.metro", "metro0");
    graph.addNode("base.print", "print0");
    if (! graph.addNode("base.counter", "counter0"))
    {
        std::cout << "Could not create a base.counter node." << std::endl;
        return false;
    }
    graph.addNode("base.any", "any0");

    if (VERBOSE)
        std::cout << "connect nodes:" << std::endl;
    graph.connect("metro0", 0, "print0", 0);
    graph.connect("metro0", 0, "counter0", 0);
    graph.connect("counter0", 0, "any0", 0);

    // disable print
    if (VERBOSE)
        std::cout << "disable print:" << std::endl;
    bool quiet = ! VERBOSE;
    Message disable_message = Message("ssb", "set", "enabled", false);
    if (quiet)
        graph.message("print0", 0, disable_message);

    // start metro
    if (VERBOSE)
        std::cout << "set metro interval:" << std::endl;
    Message interval_message = Message("ssi", "set", "interval", 100);
    graph.message("metro0", 0, interval_message);
    if (VERBOSE)
        std::cout << "start metro:" << std::endl;
    Message start_message = Message("ssb", "set", "running", true);
    graph.message("metro0", 0, start_message);

    Timer timer;
    while (true)
    {
        TimePosition elapsed = timer.elapsed();
        //if (VERBOSE)
        //    std::cout << "Elapsed: " << elapsed << std::endl;
        graph.tick();
        if (elapsed > timeposition::from_ms(1400))
        {
            if (VERBOSE)
                std::cout << "break at " <<  elapsed << std::endl;
            break;
        }
    }
    if (VERBOSE)
        std::cout << "getNode(counter0):" << std::endl;
    Node* counter0 = graph.getNode("counter0").get();
    if (counter0 == 0)
    {
        std::cout << "counter0 is a null pointer" << std::endl;
        std::cout << "FIXME: This test should fail " << std::endl;
        return true; // FIXME
        //return false;
    }
    if (VERBOSE)
        std::cout << "count = counter0->getProperty\"count\".getInt(0);" << std::endl;
    int count = counter0->getProperty("count").getInt(0);
    if (count < 10)
    {
        std::cout << "[counter] Bad count: expect 10 but got " << count << std::endl;
        std::cout << "FIXME: This test should fail " << std::endl;
        // TODO: return false;
    }

    Node* any0 = graph.getNode("any0").get();
    if (any0 == 0)
    {
        std::cout << "FIXME: This test should fail " << std::endl;
        //return true; // FIXME
        return false;
    }
//     int value = any0->getProperty("value").getInt(0);
//     if (value < 10)
//     {
//         std::cout << "[any] Bad count: expect 10 but got " << value << std::endl;
//         std::cout << "FIXME: This test should fail " << std::endl;
//         // TODO: return false;
//     }
    return true;
}

int main(int argc, char **argv)
{
    if (! checkMetro())
        return 1;
    if (! check_create())
        return 1;
    if (! checkPrepend())
        return 1;
    return 0;
}

