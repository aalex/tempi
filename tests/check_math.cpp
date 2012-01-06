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

static bool check_add()
{
    // Check we can instanciate it:
    NodeFactory::ptr factory(new NodeFactory);
    internals::loadInternals(factory);
    Node::ptr add_obj = factory->create("math.+");
    if (add_obj.get() == 0)
    {
        std::cout << "AddNode ptr is null" << std::endl;
        return false;
    }

    // Check we can actually use it:
    Graph graph(factory);
    if (VERBOSE)
        std::cout << "add nodes:" << std::endl;
    graph.addNode("math.+", "add0");
    if (VERBOSE)
        std::cout << "connect nodes:" << std::endl;
    // [print]
    if (VERBOSE)
    {
        graph.addNode("base.print", "print0");
        graph.connect("add0", 0, "print0", 0);
    }
    // TODO: store result in [any] or [appsink]
    //graph.addNode("base.any", "any0");
    //graph.connect("add0", 0, "any0", 0);

    graph.tick(); // init the nodes

    float right = 2.0f;
    float left = 4.0f;
    Message set_operand = Message("ssf", "set", "operand", right);
    graph.message("add0", 0, set_operand);
    Message float_mess = Message("f", left);
    graph.message("add0", 0, float_mess);
    if (VERBOSE)
    {
        std::cout << "with values: " << left  << " + " << right << std::endl;
    }


    // TODO when properties type will be dynamic and arguments deprecated
    // float result = graph.getNode("any0").getAttribute("value").getFloat(0);
    // if ( != 4.0f)
    // {
    //     std::cout << "expected 4.0f in any0 but got "
    // }

    graph.tick();

    return true;
}

static bool check_divide()
{
    // Check we can instanciate it:
    NodeFactory::ptr factory(new NodeFactory);
    internals::loadInternals(factory);
    Node::ptr add_obj = factory->create("math./");
    if (add_obj.get() == 0)
    {
        std::cout << "DivideNode ptr is null" << std::endl;
        return false;
    }

    // Check we can actually use it:
    Graph graph(factory);
    if (VERBOSE)
        std::cout << "divide nodes:" << std::endl;
    graph.addNode("math./", "div0");
    if (VERBOSE)
        std::cout << "connect nodes:" << std::endl;
    // [print]
    if (VERBOSE)
    {
        graph.addNode("base.print", "print0");
        graph.connect("div0", 0, "print0", 0);
    }
    // TODO: store result in [any] or [appsink]
    //graph.addNode("base.any", "any0");
    //graph.connect("add0", 0, "any0", 0);

    graph.tick(); // init the nodes

    float right = 2.0f;
    float left = 4.0f;
    Message set_operand = Message("ssf", "set", "operand", right);
    graph.message("div0", 0, set_operand);
    Message float_mess = Message("f", left);
    graph.message("div0", 0, float_mess);
    if (VERBOSE)
    {
        std::cout << "with values: " << left  << " / " << right << std::endl;
    }

    // TODO when properties type will be dynamic and arguments deprecated
    // float result = graph.getNode("any0").getProperty("value").getFloat(0);
    // if ( != 4.0f)
    // {
    //     std::cout << "expected 4.0f in any0 but got "
    // }

    graph.tick();

    return true;
}

static bool check_equalsnot()
{
    // Check we can instanciate it:
    NodeFactory::ptr factory(new NodeFactory);
    internals::loadInternals(factory);
    Node::ptr add_obj = factory->create("math.!=");
    if (add_obj.get() == 0)
    {
        std::cout << "IsEqualNode ptr is null" << std::endl;
        return false;
    }

    // Check we can actually use it:
    Graph graph(factory);
    if (VERBOSE)
        std::cout << "compare nodes for equality:" << std::endl;
    graph.addNode("math.==", "iseq0");
    if (VERBOSE)
        std::cout << "connect nodes:" << std::endl;
    // [print]
    if (VERBOSE)
    {
        graph.addNode("base.print", "print0");
        graph.connect("iseq0", 0, "print0", 0);
    }
    // TODO: store result in [any] or [appsink]
    //graph.addNode("base.any", "any0");
    //graph.connect("add0", 0, "any0", 0);

    graph.tick(); // init the nodes

    float right = 2.0f;
    float left = 3.0f;
    Message set_operand = Message("ssf", "set", "operand", right);
    graph.message("iseq0", 0, set_operand);
    Message float_mess = Message("f", left);
    graph.message("iseq0", 0, float_mess);
    if (VERBOSE)
    {
        std::cout << "with values: " << left  << " == " << right << std::endl;
    }

    // TODO when properties type will be dynamic and arguments deprecated
    // float result = graph.getNode("any0").getProperty("value").getFloat(0);
    // if ( != 4.0f)
    // {
    //     std::cout << "expected 4.0f in any0 but got "
    // }

    graph.tick();

    return true;
}

static bool check_isequal_false()
{
    // Check we can instanciate it:
    NodeFactory::ptr factory(new NodeFactory);
    internals::loadInternals(factory);
    Node::ptr add_obj = factory->create("math.==");
    if (add_obj.get() == 0)
    {
        std::cout << "IsEqualNode ptr is null" << std::endl;
        return false;
    }

    // Check we can actually use it:
    Graph graph(factory);
    if (VERBOSE)
        std::cout << "compare nodes for equality:" << std::endl;
    graph.addNode("math.==", "iseq0");
    if (VERBOSE)
        std::cout << "connect nodes:" << std::endl;
    // [print]
    if (VERBOSE)
    {
        graph.addNode("base.print", "print0");
        graph.connect("iseq0", 0, "print0", 0);
    }
    // TODO: store result in [any] or [appsink]
    //graph.addNode("base.any", "any0");
    //graph.connect("add0", 0, "any0", 0);

    graph.tick(); // init the nodes

    float right = 2.0f;
    float left = 3.0f;
    Message set_operand = Message("ssf", "set", "operand", right);
    graph.message("iseq0", 0, set_operand);
    Message float_mess = Message("f", left);
    graph.message("iseq0", 0, float_mess);
    if (VERBOSE)
    {
        std::cout << "with values: " << left  << " == " << right << std::endl;
    }

    // TODO when properties type will be dynamic and arguments deprecated
    // float result = graph.getNode("any0").getProperty("value").getFloat(0);
    // if ( != 4.0f)
    // {
    //     std::cout << "expected 4.0f in any0 but got "
    // }

    graph.tick();

    return true;
}

static bool check_isequal_true()
{
    // Check we can instanciate it:
    NodeFactory::ptr factory(new NodeFactory);
    internals::loadInternals(factory);
    Node::ptr add_obj = factory->create("math.==");
    if (add_obj.get() == 0)
    {
        std::cout << "IsEqualNode ptr is null" << std::endl;
        return false;
    }

    // Check we can actually use it:
    Graph graph(factory);
    if (VERBOSE)
        std::cout << "compare nodes for equality:" << std::endl;
    graph.addNode("math.==", "iseq0");
    if (VERBOSE)
        std::cout << "connect nodes:" << std::endl;
    // [print]
    if (VERBOSE)
    {
        graph.addNode("base.print", "print0");
        graph.connect("iseq0", 0, "print0", 0);
    }
    // TODO: store result in [any] or [appsink]
    //graph.addNode("base.any", "any0");
    //graph.connect("add0", 0, "any0", 0);

    graph.tick(); // init the nodes

    float right = 2.0f;
    float left = 2.0f;
    Message set_operand = Message("ssf", "set", "operand", right);
    graph.message("iseq0", 0, set_operand);
    Message float_mess = Message("f", left);
    graph.message("iseq0", 0, float_mess);
    if (VERBOSE)
    {
        std::cout << "with values: " << left  << " == " << right << std::endl;
    }

    // TODO when properties type will be dynamic and arguments deprecated
    // float result = graph.getNode("any0").getProperty("value").getFloat(0);
    // if ( != 4.0f)
    // {
    //     std::cout << "expected 4.0f in any0 but got "
    // }

    graph.tick();

    return true;
}

static bool check_isgreater_false()
{
    // Check we can instanciate it:
    NodeFactory::ptr factory(new NodeFactory);
    internals::loadInternals(factory);
    Node::ptr add_obj = factory->create("math.>");
    if (add_obj.get() == 0)
    {
        std::cout << "IsGreaterNode ptr is null" << std::endl;
        return false;
    }

    // Check we can actually use it:
    Graph graph(factory);
    if (VERBOSE)
        std::cout << "check if greater than:" << std::endl;
    graph.addNode("math.>", "isgr0");
    if (VERBOSE)
        std::cout << "connect nodes:" << std::endl;
    // [print]
    if (VERBOSE)
    {
        graph.addNode("base.print", "print0");
        graph.connect("isgr0", 0, "print0", 0);
    }
    // TODO: store result in [any] or [appsink]
    //graph.addNode("base.any", "any0");
    //graph.connect("add0", 0, "any0", 0);

    graph.tick(); // init the nodes

    float right = 2.0f;
    float left = 2.0f;
    Message set_operand = Message("ssf", "set", "operand", right);
    graph.message("isgr0", 0, set_operand);
    Message float_mess = Message("f", left);
    graph.message("isgr0", 0, float_mess);
    if (VERBOSE)
    {
        std::cout << "with values: " << left  << " > " << right << std::endl;
    }
    // TODO when properties type will be dynamic and arguments deprecated
    // float result = graph.getNode("any0").getProperty("value").getFloat(0);
    // if ( != 4.0f)
    // {
    //     std::cout << "expected 4.0f in any0 but got "
    // }

    graph.tick();

    return true;
}

static bool check_isgreater_true()
{
    // Check we can instanciate it:
    NodeFactory::ptr factory(new NodeFactory);
    internals::loadInternals(factory);
    Node::ptr add_obj = factory->create("math.>");
    if (add_obj.get() == 0)
    {
        std::cout << "IsGreaterNode ptr is null" << std::endl;
        return false;
    }

    // Check we can actually use it:
    Graph graph(factory);
    if (VERBOSE)
        std::cout << "check if greater than:" << std::endl;
    graph.addNode("math.>", "isgr0");
    if (VERBOSE)
        std::cout << "connect nodes:" << std::endl;
    // [print]
    if (VERBOSE)
    {
        graph.addNode("base.print", "print0");
        graph.connect("isgr0", 0, "print0", 0);
    }
    // TODO: store result in [any] or [appsink]
    //graph.addNode("base.any", "any0");
    //graph.connect("add0", 0, "any0", 0);

    graph.tick(); // init the nodes
    float right = 2.0f;
    float left = 3.0f;
    Message set_operand = Message("ssf", "set", "operand", right);
    graph.message("isgr0", 0, set_operand);
    Message float_mess = Message("f", left);
    graph.message("isgr0", 0, float_mess);
    if (VERBOSE)
    {
        std::cout << "with values: " << left  << " > " << right << std::endl;
    }
    // TODO when properties type will be dynamic and arguments deprecated
    // float result = graph.getNode("any0").getProperty("value").getFloat(0);
    // if ( != 4.0f)
    // {
    //     std::cout << "expected 4.0f in any0 but got "
    // }

    graph.tick();

    return true;
}

static bool check_isless_false()
{
    // Check we can instanciate it:
    NodeFactory::ptr factory(new NodeFactory);
    internals::loadInternals(factory);
    Node::ptr add_obj = factory->create("math.<");
    if (add_obj.get() == 0)
    {
        std::cout << "IsLessNode ptr is null" << std::endl;
        return false;
    }

    // Check we can actually use it:
    Graph graph(factory);
    if (VERBOSE)
        std::cout << "check if less than:" << std::endl;
    graph.addNode("math.<", "isgr0");
    if (VERBOSE)
        std::cout << "connect nodes:" << std::endl;
    // [print]
    if (VERBOSE)
    {
        graph.addNode("base.print", "print0");
        graph.connect("isgr0", 0, "print0", 0);
    }
    // TODO: store result in [any] or [appsink]
    //graph.addNode("base.any", "any0");
    //graph.connect("add0", 0, "any0", 0);

    graph.tick(); // init the nodes

    float right = 2.0f;
    float left = 2.0f;
    Message set_operand = Message("ssf", "set", "operand", right);
    graph.message("isgr0", 0, set_operand);
    Message float_mess = Message("f", left);
    graph.message("isgr0", 0, float_mess);
    if (VERBOSE)
    {
        std::cout << "with values: " << left  << " < " << right << std::endl;
    }
    // TODO when properties type will be dynamic and arguments deprecated
    // float result = graph.getNode("any0").getProperty("value").getFloat(0);
    // if ( != 4.0f)
    // {
    //     std::cout << "expected 4.0f in any0 but got "
    // }

    graph.tick();

    return true;
}

static bool check_isless_true()
{
    // Check we can instanciate it:
    NodeFactory::ptr factory(new NodeFactory);
    internals::loadInternals(factory);
    Node::ptr add_obj = factory->create("math.<");
    if (add_obj.get() == 0)
    {
        std::cout << "IsLessNode ptr is null" << std::endl;
        return false;
    }

    // Check we can actually use it:
    Graph graph(factory);
    if (VERBOSE)
        std::cout << "check if less than:" << std::endl;
    graph.addNode("math.<", "isgr0");
    if (VERBOSE)
        std::cout << "connect nodes:" << std::endl;
    // [print]
    if (VERBOSE)
    {
        graph.addNode("base.print", "print0");
        graph.connect("isgr0", 0, "print0", 0);
    }
    // TODO: store result in [any] or [appsink]
    //graph.addNode("base.any", "any0");
    //graph.connect("add0", 0, "any0", 0);

    graph.tick(); // init the nodes
    float right = 3.0f;
    float left = 2.0f;
    Message set_operand = Message("ssf", "set", "operand", right);
    graph.message("isgr0", 0, set_operand);
    Message float_mess = Message("f", left);
    graph.message("isgr0", 0, float_mess);
    if (VERBOSE)
    {
        std::cout << "with values: " << left  << " < " << right << std::endl;
    }
    // TODO when properties type will be dynamic and arguments deprecated
    // float result = graph.getNode("any0").getProperty("value").getFloat(0);
    // if ( != 4.0f)
    // {
    //     std::cout << "expected 4.0f in any0 but got "
    // }

    graph.tick();

    return true;
}
static bool check_multiply()
{
    // Check we can instanciate it:
    NodeFactory::ptr factory(new NodeFactory);
    internals::loadInternals(factory);
    Node::ptr add_obj = factory->create("math.*");
    if (add_obj.get() == 0)
    {
        std::cout << "MultNode ptr is null" << std::endl;
        return false;
    }

    // Check we can actually use it:
    Graph graph(factory);
    if (VERBOSE)
        std::cout << "multiply nodes:" << std::endl;
    graph.addNode("math.*", "mult0");
    if (VERBOSE)
        std::cout << "connect nodes:" << std::endl;
    // [print]
    if (VERBOSE)
    {
        graph.addNode("base.print", "print0");
        graph.connect("mult0", 0, "print0", 0);
    }
    // TODO: store result in [any] or [appsink]
    //graph.addNode("base.any", "any0");
    //graph.connect("add0", 0, "any0", 0);

    graph.tick(); // init the nodes

    float right = 2.0f;
    float left = 3.0f;
    Message set_operand = Message("ssf", "set", "operand", right);
    graph.message("mult0", 0, set_operand);
    Message float_mess = Message("f", left);
    graph.message("mult0", 0, float_mess);
    if (VERBOSE)
    {
        std::cout << "with values: " << left  << " * " << right << std::endl;
    }

    // TODO when properties type will be dynamic and arguments deprecated
    // float result = graph.getNode("any0").getProperty("value").getFloat(0);
    // if ( != 4.0f)
    // {
    //     std::cout << "expected 4.0f in any0 but got "
    // }

    graph.tick();

    return true;
}

static bool check_subtract()
{
    // Check we can instanciate it:
    NodeFactory::ptr factory(new NodeFactory);
    internals::loadInternals(factory);
    Node::ptr add_obj = factory->create("math.-");
    if (add_obj.get() == 0)
    {
        std::cout << "SubtractNode ptr is null" << std::endl;
        return false;
    }

    // Check we can actually use it:
    Graph graph(factory);
    if (VERBOSE)
        std::cout << "subtract nodes:" << std::endl;
    graph.addNode("math.-", "sub0");
    if (VERBOSE)
        std::cout << "connect nodes:" << std::endl;
    // [print]
    if (VERBOSE)
    {
        graph.addNode("base.print", "print0");
        graph.connect("sub0", 0, "print0", 0);
    }
    // TODO: store result in [any] or [appsink]
    //graph.addNode("base.any", "any0");
    //graph.connect("add0", 0, "any0", 0);

    graph.tick(); // init the nodes

    float right = 2.0f;
    float left = 3.0f;
    Message set_operand = Message("ssf", "set", "operand", right);
    graph.message("sub0", 0, set_operand);
    Message float_mess = Message("f", left);
    graph.message("sub0", 0, float_mess);
    if (VERBOSE)
    {
        std::cout << "with values: " << left  << " - " << right << std::endl;
    }

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

