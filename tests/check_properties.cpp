#include "tempi/node.h"
#include <string>
#include <iostream>

using namespace tempi;

static const bool VERBOSE = true;

class DummyNode : public Node
{
    public:
        DummyNode() :
            Node(),
            triggered_(false)
        {
        }
        bool triggered_;
    private:
        virtual void onPropertyChanged(const char *name, const Message &value)
        {
            triggered_ = true;
        }
        virtual void processMessage(unsigned int, const Message &message)
        {
        }
};

static bool check_properties()
{
    DummyNode n;

    Message message_a;
    message_a.appendInt(2);
    message_a.appendFloat(3.14159f);
    message_a.appendString("foo");

    n.addProperty("hello", message_a);

    Message message_b;
    message_b.appendInt(3);
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
        message_c.appendInt(3);
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

    return true;
}

int main(int /* argc */, char ** /*argv */)
{
    if (! check_properties())
        return 1;

    return 0;
}

