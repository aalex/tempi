#include "tempi/tempi.h"
#include "tempi/graph.h"
#include "tempi/internals.h"
#include <unistd.h>
#include <iostream>

using namespace tempi;
static const bool VERBOSE = true;
//static const bool VERBOSE = false;

static bool check_op(const char *op, float left, float right)
{
    // Check that we can associate it
    NodeFactory::ptr factory(new NodeFactory);
    internals::loadInternals(factory);
    Node::ptr add_obj = factory->create(op);
    if (add_obj.get() == 0)
    {
        std::cout << op << " ptr is null" << std::endl;
        return false;
    }
    Graph graph(factory);
    if (VERBOSE)
        std::cout << "add nodes:" << std::endl;
    graph.addNode(op, "add0");
    if (VERBOSE)
        std::cout << "connect nodes:" << std::endl;
    // [print]
    if (VERBOSE)
    {
        graph.addNode("base.print", "print0");
        graph.connect("add0", 0, "print0", 0);
    }
    graph.tick(); // init the nodes

    Message set_operand = Message("ssf", "set", "operand", right);
    graph.message("add0", 0, set_operand);
    Message float_mess = Message("f", left);
    graph.message("add0", 0, float_mess);
    if (VERBOSE)
    {
        std::cout << "with values: " << op << " " << left << " " \
		  << right << std::endl;
    }
    return true;
}

int main(int argc, char **argv)
{
    const char * add = "math.+";
    const char * divide = "math./";
    const char * equal = "math.==";
    const char * notequal = "math.!=";
    const char * greater = "math.>";
    const char * less = "math.<";
    const char * multiply = "math.*";
    const char * subtract = "math.-";
    if (! check_op(add, 2.0f, 2.0f))
        return 1;
    if (! check_op(divide, 4.0f, 2.0f))
        return 1;
    if (! check_op(equal, 1.0f, 1.0f))
        return 1;
    if (! check_op(notequal, 1.0f, 0.0f))
        return 1;
    if (! check_op(greater, 4.0f, 2.0f))
        return 1;
    if (! check_op(less, 1.0f, 2.0f))
        return 1;
    if (! check_op(multiply, 2.0f, 2.0f))
        return 1;
    if (! check_op(subtract, 2.0f, 2.0f))
        return 1;
    return 0;

}

