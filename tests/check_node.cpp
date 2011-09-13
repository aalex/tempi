#include <iostream>
#include "tempi/tempi.h"
#include "tempi/node.h"
#include "tempi/sink.h"
#include "tempi/source.h"

using namespace tempi;

/**
 * Dummy source node.
 */
class DummySourceNode: public Node
{
    public:
        DummySourceNode();
    private:
        virtual void processMessage(Source *source, boost::any data);
};

DummySourceNode::DummySourceNode() : Node() {}
void DummySourceNode::processMessage(Source *source, boost::any data) {}

/**
 * Dummy sink node.
 */
class DummySinkNode: public Node
{
    public:
        DummySinkNode();
        bool triggered_;
    private:
        virtual void processMessage(Source *source, boost::any data);
};

DummySinkNode::DummySinkNode() : Node()
{
    triggered_ = false;
}

void DummySinkNode::processMessage(Source *source, boost::any data)
{
    triggered_ = true;
}


bool check_simple()
{
    DummySourceNode a;
    DummySinkNode b;
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
        std::cout << "Could not connect." << std::endl;
        return false;
    }
    Source *source = a.getOutlets()[0].get();
    boost::any data(true);
    if (b.triggered_)
    {
        std::cout << "Callback has already been called. Weird" << std::endl;
        return false;
    }
    source->trigger(data);
    if (! b.triggered_)
    {
        std::cout << "Callback has not been called." << std::endl;
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

