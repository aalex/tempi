#include <iostream>
#include <boost/any.hpp>
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
        virtual void processMessage(Source *source, boost::any data);
};

DummySourceNode::DummySourceNode() : SourceNode() {}
void DummySourceNode::processMessage(Source *source, boost::any data) {}

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
        virtual boost::any filter(boost::any data);
};

DummyFilterNode::DummyFilterNode() : Filter() {}

boost::any DummyFilterNode::filter(boost::any data)
{
    return boost::any(! boost::any_cast<bool>(data)); // pass through
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
        virtual void processMessage(Source *source, boost::any data);
};

DummySinkNode::DummySinkNode() :
    SinkNode()
{
    triggered_ = false;
    expected_ = false;
    last_value_was_ok_ = false;
}

void DummySinkNode::processMessage(Source *source, boost::any data)
{
    triggered_ = true;
    last_value_was_ok_ = boost::any_cast<bool>(data) == expected_;
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
    boost::any data(true);
    if (c.triggered_)
    {
        std::cout << "Callback has already been called. Weird" << std::endl;
        return false;
    }
    c.expected_ = false;
    source->trigger(data);
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

int main(int argc, char *argv[])
{
    if (not check_simple())
        return 1;
    return 0;
}

