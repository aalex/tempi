#include "tempi/node.h"
#include <string>
#include <iostream>

using namespace tempi;

static const bool VERBOSE = false;

class DummyNode : public Node
{
    public:
        DummyNode() :
            Node(),
            triggered_(false)
        {
            Message message_a = Message("ifs", 1, 3.14159f, "foo");
            addProperty("hello", message_a);
        }
        bool triggered_;
    private:
        virtual void onPropertyChanged(const char *name, const Message &value)
        {
            triggered_ = true;
            if (VERBOSE)
            {
                std::cout << "DummyNode::" << __FUNCTION__ << " " << name << "=" << value << std::endl;
                std::cout << "DummyNode::triggered_ = " << triggered_ << std::endl;
            }
        }
        virtual void processMessage(unsigned int inlet, const Message &message)
        {
            if (VERBOSE)
                std::cout << __FUNCTION__ << " " << inlet << " " << message << std::endl;
        }
};

static bool check_properties()
{
    DummyNode n;

    Message message_b;
    message_b.appendInt(2);
    message_b.appendFloat(6.819f);
    message_b.appendString("bar");

    n.setProperty("hello", message_b);
    if (! n.triggered_)
    {
        std::cout << "property not triggered" << std::endl;
        return false;
    }

    try
    {
        Message message_c;
        message_c.appendInt(9);
        n.setProperty("hello", message_c);
        std::cout << "Should not be able to set a property with a different type.\n";
        return false;
    }
    catch (const BadArgumentTypeException &e)
    {}

    try
    {
        n.setProperty("invalid", message_b);
        std::cout << "Should not be able to set a property that does not exist.\n";
        return false;
    }
    catch (const BadIndexException &e)
    {}

    Message set_message = Message("ssifs", "set", "hello", 3, 9.124351f, "qweqweqweqw");
    n.triggered_ = false;
    n.getInlet(0)->trigger(set_message);
    if (VERBOSE)
    {
        std::cout << n.triggered_ << std::endl;
        std::cout << n.getProperty("hello") << std::endl;
    }
    if (! n.triggered_)
    {
        std::cout << "property not triggered (using a set message)" << std::endl;
        return false;
    }
    return true;
}

int main(int /* argc */, char ** /*argv */)
{
    if (! check_properties())
        return 1;

    return 0;
}

