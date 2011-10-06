#include <iostream>
#include "tempi/tempi.h"

using namespace tempi;

/**
 * Dummy node.
 */
class DummyNode: public Node
{
    public:
        DummyNode() :
            Node()
        {
            addInlet();
            addOutlet();
        }
    private:
        virtual void processMessage(unsigned int inlet, const Message &message)
        {
            getOutlet(0)->trigger(message);
            std::cout << "DummyNode::" << __FUNCTION__ << ": " << message << std::endl;
        }
};

static std::tr1::shared_ptr<Node> makeDummyNode()
{
    return std::tr1::shared_ptr<Node>(new DummyNode);
}

bool check_graph()
{
    Graph graph;

    graph.addNode("source0", makeDummyNode());
    graph.addNode("filter0", makeDummyNode());
    graph.addNode("sink0", makeDummyNode());
    graph.addNode("sink1", makeDummyNode());

    graph.connect("source0", 0, "filter0", 0);
    graph.connect("filter0", 0, "sink0", 0);
    graph.connect("filter0", 0, "sink1", 0);

    for (int i = 0; i < 10; ++i)
        graph.tick();

    Message message;
    message.appendFloat(3.14159f);
    message.appendInt(2);
    message.appendString("hello");
    graph.message("source0", 0, message);

    return true;
}

int main(int argc, char *argv[])
{
    if (not check_graph())
        return 1;
    return 0;
}

