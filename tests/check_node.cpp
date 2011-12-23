#include <iostream>
#include "tempi/node.h"
#include "tempi/base/nop_node.h"

using namespace tempi;

/**
 * Dummy sink node.
 */
class DummyInletNode: public base::NopNode
{
    public:
        DummyInletNode() :
            base::NopNode()
        {
            triggered_ = false;
            expected_ = false;
            last_value_was_ok_ = false;
            addOutlet();
        }
        bool triggered_;
        bool last_value_was_ok_;
        bool expected_;
    private:
        virtual void processMessage(unsigned int inlet, const Message &message)
        {
            triggered_ = true;
            bool value = message.getBoolean(0);
            last_value_was_ok_ = value == expected_;
        }
};

bool check_simple()
{
    base::NopNode a;
    base::NopNode b;
    DummyInletNode c;
    if (! b.getInlets()[0].get()->connect(a.getOutlets()[0]))
    {
        std::cout << "Could not connect a to b." << std::endl;
        return false;
    }
    if (! c.getInlet(0)->connect(b.getOutlets()[0]))
    {
        std::cout << "Could not connect b to c." << std::endl;
        return false;
    }
    Outlet *source = a.getOutlets()[0].get();
    Message message = Message("b", true);
    if (c.triggered_)
    {
        std::cout << "Callback has already been called. Weird" << std::endl;
        return false;
    }
    c.expected_ = true;
    source->trigger(message);
    if (! c.triggered_)
    {
        std::cout << "Callback has not been called." << std::endl;
        return false;
    }
    if (! c.last_value_was_ok_)
    {
        std::cout << "Last value was not what was expected." << std::endl;
        return false;
    }

    if (! c.getInlet(0)->disconnect(b.getOutlets()[0]))
    {
        std::cout << "Could not disconnect b to c." << std::endl;
        return false;
    }
    if (c.getInlet(0)->disconnect(b.getOutlets()[0]))
    {
        std::cout << "Could disconnect b to c twice." << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (! check_simple())
        return 1;
    return 0;
}

