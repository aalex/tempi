#include <iostream>
#include "tempi/tempi.h"
#include "tempi/node.h"
#include "tempi/sink.h"
#include "tempi/source.h"

using namespace tempi;

class DummyNode: public Node
{
    private:
        virtual void processTrigger(Source *source, boost::any data);
}

void DummyNode::processTrigger(Source *source, boost::any data)
{
    // pass
}

bool check_simple()
{
    DummyNode a;
    DummyNode b;
    if (! a.addSource(SourcePtr(new Source())))
    {
        std::cout << "Could not add source." << std::endl;
        return false;
    }
    if (! b.addSink(SinkPtr(new Sink())))
    {
        std::cout << "Could not add sink." << std::endl;
        return false;
    }
    if (! b.getSinks()[0].get()->connect(a.getSources()[0]))
    {
        std::cout << "Could not connect." << std::endl;
        return false;
    }
    Source *source = a.getSources()[0].get();
    boost::any data(true);
    source->trigger(data);
    return true;
}

int main(int argc, char *argv[])
{
    if (not check_simple())
        return 1;
    return 0;
}

