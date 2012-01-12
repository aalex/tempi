#include "tempi/tempi.h"
#include "tempi/graph.h"
#include "tempi/internals.h"
#include <unistd.h>
#include <iostream>
#include <string>

using namespace tempi;
static const bool VERBOSE = true;
//static const bool VERBOSE = false;

static bool check_op(const char *op, float left, float right)
{
    // Check that we can create the given node type.
    NodeFactory::ptr factory(new NodeFactory);
    internals::loadInternals(factory);
    Node::ptr obj = factory->create(op);
    if (obj.get() == 0)
    {
        std::cout << op << " ptr is null" << std::endl;
        return false;
    }

    // Create a Graph with a few nodes to check if calculation is OK.
    Graph graph(factory);
    if (VERBOSE)
        std::cout << "add nodes:" << std::endl;
    graph.addNode(op, "op0");
    // [print]
    if (VERBOSE)
        std::cout << "create print node:" << std::endl;
    if (VERBOSE)
    {
        graph.addNode("base.print", "print0");
        if (VERBOSE)
            std::cout << "connect print node:" << std::endl;
        graph.connect("op0", "0", "print0", "0");
    }
    graph.tick(); // init the nodes

    // Set the operand and trigger the calculation.
    if (VERBOSE)
        std::cout << "set operand: " << std::endl;
    Message set_operand = Message("ssf", "set", "operand", right);
    graph.message("op0", "attributes", set_operand);
    if (VERBOSE)
        std::cout << "send float to hot inlet : " << std::endl;
    Message float_mess = Message("f", 2.0f);
    graph.message("op0", "0", float_mess);
    if (VERBOSE)
    {
        std::cout << "with values: " << op << " " << left << " " \
		  << right << std::endl;
    }
    graph.tick();

    // FIXME: this doesn't check anything at all!

    return true;
}

int main(int argc, char **argv)
{
    std::string add = std::string("math.+");
    std::string divide= std::string("math./");
    std::string equal = std::string("math.==");
    std::string notequal = std::string("math.!=");
    std::string greater = std::string("math.>");
    std::string less = std::string("math.<");
    std::string multiply = std::string("math.*");
    std::string subtract = std::string("math.-");

    if (! check_op(add.c_str(), 2.0f, 2.0f))
        return 1;
    if (! check_op(divide.c_str(), 4.0f, 2.0f))
        return 1;
    if (! check_op(equal.c_str(), 1.0f, 1.0f))
        return 1;
    if (! check_op(notequal.c_str(), 1.0f, 0.0f))
        return 1;
    if (! check_op(greater.c_str(), 4.0f, 2.0f))
        return 1;
    if (! check_op(less.c_str(), 1.0f, 2.0f))
        return 1;
    if (! check_op(multiply.c_str(), 2.0f, 2.0f))
        return 1;
    if (! check_op(subtract.c_str(), 2.0f, 2.0f))
        return 1;
    return 0;

}

