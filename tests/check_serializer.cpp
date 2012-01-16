#include "tempi/serializer.h"
#include "tempi/graph.h"
#include "tempi/nodefactory.h"
#include "tempi/internals.h"
#include <unistd.h>
#include <iostream>

using namespace tempi;
static const bool VERBOSE = false;

static bool check_save()
{
    NodeFactory::ptr factory = NodeFactory::ptr(new NodeFactory);
    internals::loadInternals(factory);
    Graph graph(factory);
    graph.addNode("base.metro", "metro0");
    graph.addNode("base.print", "print0");
    graph.tick();

    graph.connect("metro0", "0", "print0", "0");
    graph.message("print0", "attributes", Message("ssb", "set", "enabled", false));

    graph.tick();

    std::string file_name("/tmp/tempi-example-graph.xml");
    if (VERBOSE)
        std::cout << "Saving to " << file_name << std::endl;
    serializer::Serializer saver;
    saver.save(graph, file_name.c_str());
    return true;
}

int main(int argc, char **argv)
{
    if (! check_save())
        return 1;
    return 0;
}

