#include "tempi/tempi.h"
#include "tempi/graph.h"
#include "tempi/internals.h"
#include <unistd.h>
#include <iostream>

using namespace tempi;
static const bool VERBOSE = false;

static bool check_add()
{
    // Check we can instanciate it:
    NodeFactory::ptr factory(new NodeFactory);
    internals::loadInternals(factory);
    Node::ptr add_obj = factory->create("math.add");
    if (add_obj.get() == 0)
    {
        std::cout << "AddNode ptr is null" << std::endl;
        return false;
    }

    // Check we can actually use it:
    Graph graph(factory);
    if (VERBOSE)
        std::cout << "add nodes:" << std::endl;
    graph.addNode("math.add", "add0");
    graph.addNode("base.print", "print0");
    graph.addNode("base.any", "any0");
    if (VERBOSE)
        std::cout << "connect nodes:" << std::endl;
    graph.connect("add0", 0, "print0", 0);
    graph.connect("add0", 0, "any0", 0);

    graph.tick(); // init the nodes

    Message set_operand = Message("ssf", "set", "operand", 2.0f);
    graph.message("add0", 0, set_operand);
    Message float_mess = Message("f", 2.0f);
    graph.message("add0", 0, float_mess);

    // TODO when properties type will be dynamic and arguments deprecated
    // float result = graph.getNode("any0").getProperty("value").getFloat(0);
    // if ( != 4.0f)
    // {
    //     std::cout << "expected 4.0f in any0 but got "
    // }

    graph.tick();

    return true;
}

int main(int argc, char **argv)
{
    if (! check_add())
        return 1;
    return 0;
}

