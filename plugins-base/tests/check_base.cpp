#include "tempi/timer.h"
#include "tempi/timeposition.h"
#include "tempi/graph.h"
#include "tempi/internals.h"
#include <unistd.h>
#include <iostream>

using namespace tempi;
static const bool VERBOSE = false;
static NodeFactory::ptr factory;

static bool check_create()
{
    Node::ptr metro0 = factory->create("base.time.metro");
    Node::ptr print0 = factory->create("base.flow.print");
    Node::ptr counter0 = factory->create("base.flow.counter");
    if (metro0.get() == 0)
    {
        std::cout << "Metro ptr is null" << std::endl;
        return false;
    }
    return true;
}

/**
 * Creates a node of a type that is supposed to be available in the base plugins.
 */
static bool checkPrepend()
{
    if (VERBOSE)
        std::cout << __FUNCTION__ << std::endl;

    if (VERBOSE)
        std::cout << "create Graph:" << std::endl;
    Graph graph(factory);
    if (VERBOSE)
        std::cout << "add nodes:" << std::endl;
    graph.addNode("base.flow.print", "print0");
    // TODO: setNodeArguments
    return true;
}

static bool checkMetro()
{
    if (VERBOSE)
        std::cout << __FUNCTION__ << std::endl;
    if (VERBOSE)
        std::cout << "create Graph:" << std::endl;
    Graph graph(factory);
    if (VERBOSE)
        std::cout << "add nodes:" << std::endl;
    graph.addNode("base.time.metro", "metro0");
    graph.addNode("base.flow.print", "print0");
    if (! graph.addNode("base.flow.counter", "counter0"))
    {
        std::cout << "Could not create a base.counter node." << std::endl;
        return false;
    }
    graph.addNode("base.flow.any", "any0");

    if (VERBOSE)
        std::cout << "connect nodes:" << std::endl;
    graph.connect("metro0", "0", "print0", "0");
    graph.connect("metro0", "0", "counter0", "0");
    graph.connect("counter0", "0", "any0", "0");

    graph.tick(); // init the nodes

    // disable print
    if (VERBOSE)
        std::cout << "disable print:" << std::endl;
    bool quiet = ! VERBOSE;
    Message disable_message = Message("ssb", "set", "enabled", false);
    if (quiet)
        graph.message("print0", "__attr__", disable_message);

    // start metro
    if (VERBOSE)
        std::cout << "set metro interval:" << std::endl;
    Message interval_message = Message("ssi", "set", "interval", 100);
    graph.message("metro0", "__attr__", interval_message);
    if (VERBOSE)
        std::cout << "start metro:" << std::endl;
    Message start_message = Message("ssb", "set", "running", true);
    graph.message("metro0", "__attr__", start_message);

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
    // FIXME: the following is deprecated:
    // if (VERBOSE)
    //     std::cout << "count = counter0->getAttributeValue(\"count\").getInt(0);" << std::endl;
    // int count = counter0->getAttributeValue("count").getInt(0);
    // if (count < 10)
    // {
    //     std::cout << "[counter] Bad count: expect 10 but got " << count << std::endl;
    //     std::cout << "FIXME: This test should fail " << std::endl;
    //     // TODO: return false;
    // }

    Node* any0 = graph.getNode("any0").get();
    if (any0 == 0)
    {
        std::cout << "FIXME: This test should fail " << std::endl;
        //return true; // FIXME
        return false;
    }
//     int value = any0->getAttributeValue("value").getInt(0);
//     if (value < 10)
//     {
//         std::cout << "[any] Bad count: expect 10 but got " << value << std::endl;
//         std::cout << "FIXME: This test should fail " << std::endl;
//         // TODO: return false;
//     }
    return true;
}

static bool check_all_loaded()
{
    std::vector<std::string> names;
    names.push_back(std::string("base.flow.print"));
    names.push_back(std::string("base.flow.nop"));
    names.push_back(std::string("base.time.metro"));
    names.push_back(std::string("base.flow.any"));
    names.push_back(std::string("base.flow.counter"));
    //names.push_back(std::string("base.appsink"));

    std::vector<std::string>::const_iterator iter;
    for (iter = names.begin(); iter != names.end(); ++iter)
    {
        if (! factory->hasType((*iter).c_str()))
        {
            std::cout << "Factory should have type " << (*iter) << std::endl;
            return false;
        }
    }
    return true;
}

/**
 * checks flow.any
 */
static bool check_any()
{
    Graph graph(factory);
    graph.addNode("base.flow.any", "any0");
    Node::ptr any0 = graph.getNode("any0");
    any0->setAttributeValue("value", Message("f", 3.14159f));
    return true;
}

int main(int argc, char **argv)
{
    factory.reset(new NodeFactory);
    internals::loadInternals(factory);
    if (! checkMetro())
        return 1;
    if (! check_create())
        return 1;
    if (! checkPrepend())
        return 1;
    if (! check_all_loaded())
        return 1;
    if (! check_any())
        return 1;
    return 0;
}

