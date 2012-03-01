#include "tempi/tempi.h"
#include "tempi/graph.h"
#include "tempi/internals.h"
#include <unistd.h>
#include <iostream>
#include <string>

using namespace tempi;
//static const bool VERBOSE = true;
static const bool VERBOSE = false;
static NodeFactory::ptr factory;

template <typename T>
static bool check_op(const char *op, float left, float right, T expected)
{
    // Check that we can create the given node type.
    //internals::loadInternals(factory);
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
    graph.addNode("base.any", "any0");
    graph.connect("op0", "0", "any0", "0");
    graph.tick(); // init the nodes

    // Set the operand and trigger the calculation.
    if (VERBOSE)
        std::cout << "set operand: " << std::endl;
    Message set_operand = Message("ssf", "set", "operand", right);
    graph.message("op0", "__attr__", set_operand);
    if (VERBOSE)
        std::cout << "send float to hot inlet : " << std::endl;
    Message float_mess = Message("f", left);
    graph.message("op0", "0", float_mess);
    if (VERBOSE)
    {
        std::cout << "with values: " << op << " " << left << " " \
		  << right << std::endl;
    }
    graph.tick();

    T result;
    if (typeid(T) == typeid(float))
        result = graph.getNode("any0")->getAttribute("value")->getValue().getFloat(0);
    else if (typeid(T) == typeid(bool))
        result = graph.getNode("any0")->getAttribute("value")->getValue().getBoolean(0);
    bool ret = (result == expected);
    if (! ret)
    {
        std::cout << left << " " << op << " " << right << " != " << expected << ". Result is " << result << std::endl;
    }
    if (VERBOSE)
    {
        std::cout << graph;
    }
    return ret;
}

int main(int argc, char **argv)
{
    factory.reset(new NodeFactory);
    internals::loadInternals(factory);

    std::string add = std::string("math.+");
    std::string divide= std::string("math./");
    std::string equal = std::string("math.==");
    std::string notequal = std::string("math.!=");
    std::string greater = std::string("math.>");
    std::string less = std::string("math.<");
    std::string multiply = std::string("math.*");
    std::string subtract = std::string("math.-");

    if (! check_op(add.c_str(), 2.0f, 2.0f, 4.0f))
        return 1;
    // if (! check_op(divide.c_str(), 8.0f, 4.0f, 2.0f))
    //     return 1;
    if (! check_op(equal.c_str(), 1.0f, 1.0f, true))
        return 1;
    if (! check_op(notequal.c_str(), 1.0f, 0.0f, true))
        return 1;
    if (! check_op(greater.c_str(), 4.0f, 2.0f, true))
        return 1;
    if (! check_op(less.c_str(), 1.0f, 2.0f, true))
        return 1;
    if (! check_op(multiply.c_str(), 2.0f, 2.0f, 4.0f))
        return 1;
    if (! check_op(subtract.c_str(), 2.0f, 2.0f, 0.0f))
        return 1;
    return 0;

}

