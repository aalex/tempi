#include "tempi/serializer.h"
#include "tempi/graph.h"
#include "tempi/nodefactory.h"
#include "tempi/internals.h"
#include "tempi/sampler_region.h"
#include <unistd.h>
#include <iostream>

using namespace tempi;
static const bool VERBOSE = false;

static bool check_save_graph()
{
    NodeFactory::ptr factory = NodeFactory::ptr(new NodeFactory);
    internals::loadInternals(factory);
    Graph::ptr graph(new Graph(factory));
    graph->addNode("base.metro", "metro0");
    graph->addNode("base.print", "print0");
    graph->tick();

    graph->connect("metro0", "0", "print0", "0");
    graph->message("print0", "__attr__", Message("ssb", "set", "enabled", false));
    graph->message("metro0", "__attr__", Message("ssi", "set", "interval", 1000));

    graph->tick();

    std::string file_name("/tmp/tempi-example-graph.xml");
    if (VERBOSE)
        std::cout << "Saving to " << file_name << std::endl;
    serializer::Serializer saver;
    saver.save(*graph, file_name.c_str());

    graph.reset(new Graph(factory));
    saver.load(*graph, file_name.c_str());
    graph->tick(); // FIXME
    if (! graph->hasNode("metro0"))
    {
        std::cout << "Could not load metro0\n";
        return false;
    }

    if (! graph->isConnected("metro0", "0", "print0", "0"))
    {
        std::cout << "Could not load connection between metro0:0 and print0:0\n";
        return false;
    }

    Node::ptr metro0 = graph->getNode("metro0");
    if (metro0->getAttributeValue("interval").getInt(0) != 1000)
    {
        std::cout << "metro0's interval should be 1000\n";
        return false;
    }
    return true;
}

static bool check_save_region()
{
    sampler::Region region;

    region.add(0L, Message("s", "hello"));
    region.add(1000L, Message("i", 2));
    region.add(2000L, Message("f", 3.14159f));
    std::string file_name("/tmp/tempi-example-region.xml");
    if (VERBOSE)
        std::cout << "Saving to " << file_name << std::endl;
    serializer::Serializer saver;
    saver.save(region, file_name.c_str());

    return true;
}

int main(int argc, char **argv)
{
    if (! check_save_graph())
        return 1;
    if (! check_save_region())
        return 1;
    return 0;
}

