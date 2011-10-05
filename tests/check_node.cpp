#include <iostream>
#include "tempi/tempi.h"
#include "tempi/node.h"
#include "tempi/sink.h"
#include "tempi/source.h"
#include "tempi/sourcenode.h"
#include "tempi/sinknode.h"
#include "tempi/filter.h"

using namespace tempi;

/**
 * Dummy source node.
 */
class DummySourceNode: public SourceNode
{
    public:
        DummySourceNode();
    private:
        virtual void processMessage(const Message &message);
};

DummySourceNode::DummySourceNode() : SourceNode() {}
void DummySourceNode::processMessage(const Message &message) {}

/**
 * Dummy filter node.
 */
class DummyFilterNode: public Filter
{
    public:
        DummyFilterNode();
    private:
        /**
         * Inverts the boolean value.
         */
        virtual Message filter(const Message &message);
};

DummyFilterNode::DummyFilterNode() : Filter() {}

Message DummyFilterNode::filter(const Message &message)
{
    Message ret = message;
    ret.setBoolean(0, ! message.getBoolean(0));
    return ret;
}

/**
 * Dummy sink node.
 */
class DummySinkNode: public SinkNode
{
    public:
        DummySinkNode();
        bool triggered_;
        bool last_value_was_ok_;
        bool expected_;
    private:
        virtual void processMessage(const Message &message);
};

DummySinkNode::DummySinkNode() :
    SinkNode()
{
    triggered_ = false;
    expected_ = false;
    last_value_was_ok_ = false;
}

void DummySinkNode::processMessage(const Message &message)
{
    triggered_ = true;
    bool value = message.getBoolean(0);
    last_value_was_ok_ = value == expected_;
}

bool check_simple()
{
    DummySourceNode a;
    DummyFilterNode b;
    DummySinkNode c;
    if (! a.addOutlet(std::tr1::shared_ptr<Source>(new Source())))
    {
        std::cout << "Could not add outlet." << std::endl;
        return false;
    }
    if (! b.addInlet(std::tr1::shared_ptr<Sink>(new Sink())))
    {
        std::cout << "Could not add inlet." << std::endl;
        return false;
    }
    if (! b.getInlets()[0].get()->connect(a.getOutlets()[0]))
    {
        std::cout << "Could not connect a to b." << std::endl;
        return false;
    }
    if (! c.getInlet()->connect(b.getOutlets()[0]))
    {
        std::cout << "Could not connect b to c." << std::endl;
        return false;
    }
    Source *source = a.getOutlets()[0].get();
    Message message;
    message.appendBoolean(true);
    if (c.triggered_)
    {
        std::cout << "Callback has already been called. Weird" << std::endl;
        return false;
    }
    c.expected_ = false;
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
    return true;
}

// class FooNode: public Node
// {
//     public:
//         FooNode();
//         bool property_changed_triggered_;
//     private:
//         virtual void processMessage(const Message &message) {}
//         virtual void onPropertyChanged(Property &property);
// };
// 
// FooNode::FooNode() :
//     Node(),
//     property_changed_triggered_(false)
// {
//     addProperty(std::tr1::shared_ptr<Property>(new Property("hello", boost::any(_ff(1.234, 5.678)), "Description")));
//     property_changed_triggered_ = false;
// }
// 
// void FooNode::onPropertyChanged(Property &property)
// {
//     property_changed_triggered_ = true;
// }
// 
// 
// bool check_node_properties()
// {
//     FooNode foo;
//     foo.getProperty("hello").setValue<_ff>(_ff(2.345, 6.789));
//     if (! foo.property_changed_triggered_)
//     {
//         std::cout << "Property signal not triggered." << std::endl;
//         return false;
//     }
//     return true;
// }

int main(int argc, char *argv[])
{
    if (! check_simple())
        return 1;
    //if (! check_node_properties())
    //    return 1;
    return 0;
}

