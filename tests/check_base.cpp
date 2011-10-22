#include "tempi/tempi.h"
#include "tempi/graph.h"
#include <unistd.h>
#include <iostream>

using namespace tempi;
static const bool VERBOSE = false;

class CounterNode: public Node
{
    public:
        CounterNode() :
            Node()
        {
            if (VERBOSE)
                std::cout << "Create a CounterNode" << std::endl;
            addOutlet();
        }
        unsigned int getCount() const { return count_; }
    private:
        virtual void processMessage(unsigned int inlet, const Message &message)
        {
            if (message.getTypes() == "s" && message.getString(0) == "reset")
            {
                count_ = 0;
            }
            if (message.getTypes() == "i")
            {
                count_ = (unsigned int) message.getInt(0);
            }
            else
                ++ count_;
        }
        unsigned int count_;
};

bool check_metro()
{
    if (VERBOSE)
        std::cout << __FUNCTION__ << std::endl;
    NodeFactory factory;
    factory.loadInternals();
    factory.registerTypeT<CounterNode>("counter");
    Node::ptr metro0 = factory.create("metro");
    Node::ptr print0 = factory.create("print");
    Node::ptr counter0 = factory.create("counter");

    if (metro0.get() == 0 || print0.get() == 0 || counter0.get() == 0)
    {
        std::cout << __FUNCTION__ << ": invalid pointer" << std::endl;
        return false;
    }
    Graph graph;
    graph.addNode(metro0, "metro0");
    graph.addNode(print0, "print0");
    graph.addNode(counter0, "counter0");

    graph.connect("metro0", 0, "print0", 0);
    graph.connect("metro0", 0, "counter0", 0);

    // disable print
    bool quiet = ! VERBOSE;
    Message disable_message = Message("ssb", "set", "enabled", false);
    if (quiet)
        graph.message("print0", 0, disable_message);

    // start metro
    Message interval_message = Message("ssi", "set", "interval", 100);
    graph.message("metro0", 0, interval_message);
    Message start_message = Message("ssb", "set", "running", true);
    graph.message("metro0", 0, start_message);

    Timer timer;
    while (true)
    {
        TimePosition elapsed = timer.elapsed();
        if (VERBOSE)
            std::cout << "Elapsed: " << elapsed << std::endl;
        graph.tick();
        if (elapsed > timeposition::from_ms(1400))
        {
            if (VERBOSE)
                std::cout << "break at " <<  elapsed << std::endl;
            break;
        }
    }
    CounterNode *counter = (CounterNode *) counter0.get();
    if (counter->getCount() < 10)
    {
        std::cout << "Bad count: expect 10 but got " << counter->getCount() << std::endl;
        // TODO: return false;
    }
    return true;
}

int main(int argc, char **argv)
{
    if (! check_metro())
        return 1;
    return 0;
}

