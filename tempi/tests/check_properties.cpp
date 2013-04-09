#include "tempi/node.h"
#include "tempi/log.h"
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
            addAttribute(Attribute::ptr(new Attribute("hello", message_a)));
            addInlet("0");
            addOutlet("0");
        }
        bool triggered_;
    private:
        virtual bool onNodeAttributeChanged(const char *name, const Message &value)
        {
            triggered_ = true;
            if (VERBOSE)
            {
                std::cout << "DummyNode::" << __FUNCTION__ << " " << name << "=" << value << std::endl;
                std::cout << "DummyNode::triggered_ = " << triggered_ << std::endl;
            }
            return true;
        }
        virtual void processMessage(const char *inlet, const Message &message)
        {
            if (VERBOSE)
                std::cout << "DummyNode." << __FUNCTION__ << "(" << inlet << ", " << message << ")" << std::endl;
        }
};

static bool check_properties()
{
    DummyNode n;
    n.init(); // XXX Very important. Won't work if not called!!

    Message message_b;
    message_b.appendInt(2);
    message_b.appendFloat(6.819f);
    message_b.appendString("bar");

    n.setAttributeValue("hello", message_b);
    if (! n.triggered_)
    {
        std::cout << "property not triggered" << std::endl;
        return false;
    }

    try
    {
        Message message_c;
        message_c.appendInt(9);
        n.setAttributeValue("hello", message_c);
        std::cout << "Should not be able to set a property with a different type.\n";
        return false;
    }
    catch (const BadAtomTypeException &e)
    {}

    try
    {
        n.setAttributeValue("invalid", message_b);
        std::cout << "Should not be able to set a property that does not exist.\n";
        return false;
    }
    catch (const BadIndexException &e)
    {}

    Message set_message = Message("ssifs", Node::ATTRIBUTES_SET_METHOD_SELECTOR, "hello", 3, 9.124351f, "qweqweqweqw");
    // reset to false:
    n.triggered_ = false;
    // Send set message:
    // FIXME:The following doesn't work!
    //n.getInlet(Node::ATTRIBUTES_INLET)->trigger(set_message);
    // FIXME:The following doesn't work!
    n.message(Node::ATTRIBUTES_INLET, set_message);
    if (VERBOSE)
    {
        std::ostringstream os;
        os << "Did trigger attr: " << (n.triggered_ ? "true" : "false");
        os << ". Value: " << n.getAttributeValue("hello") << std::endl;
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
    if (VERBOSE)
        Logger::getInstance().setLevel(DEBUG);
    if (! check_properties())
        return 1;

    return 0;
}

