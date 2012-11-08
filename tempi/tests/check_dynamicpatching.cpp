#include <iostream>
#include "tempi/dynamicpatching.h"
#include "tempi/nodefactory.h"
#include "tempi/internals.h"

using namespace tempi;

static const bool VERBOSE = false;

static bool splits_ok(const char *path, const std::vector<std::string> &expected, bool should_be_ok)
{
    std::vector<std::string> result;
    bool ok = dynamic::split_path(path, result);
    if (! ok && ! should_be_ok)
        return true;
    if (! ok && should_be_ok)
    {
        std::cout << "split_path returned false for path " << path << std::endl;
        return false;
    }
    else if (ok && ! should_be_ok)
    {
        std::cout << "split_path returned true for path " << path << std::endl;
        return false;
    }
    else
    {
        if (result.size() != expected.size())
        {
            std::cout << "length don't match expected for path " << path <<
                ". Expected " << expected.size() <<
                ". Got " << result.size() << std::endl;
       //     return false;
        }
        for (unsigned int i = 0; i < result.size(); i++)
        {
            if (VERBOSE)
                std::cout << "Element " << i << " in result: "  << result[i] << ", in expected: " << expected[i] << "." << std::endl;
            if (result[i] != expected[i])
            {
                std::cout << "Element " << result[i] << " doesn't match element " << expected[i] << "." << std::endl;
                return false;
            }
        }
        return true;
    }
}

static bool check_split_path()
{
    {
        std::string path = "/bar/egg/spam";
        std::vector<std::string> expected;
        expected.push_back("bar");
        expected.push_back("egg");
        expected.push_back("spam");
        if (! splits_ok(path.c_str(), expected, true))
            return false;
    }
    return true;
}

static bool check_dynamic_patching(NodeFactory::ptr factory)
{
    Graph graph(factory);
    Message message = Message("sss", "addNode", "base.flow.line", "line0");
    std::vector<std::string> path;
    if (! dynamic::handle_graph_message(graph, path, message))
    {
        std::cout << "failed calling handle_graph_message " << message;
        return false;
    }
    int RATE = 100;
    message = Message("ssi", "setAttribute", "rate", RATE);
    path.push_back(std::string("line0"));
    if (! dynamic::handle_graph_message(graph, path, message))
    {
        std::cout << "failed calling handle_graph_message" << message;
        return false;
    }
    if (! graph.getNode("line0")->getAttributeValue("rate").getInt(0) == RATE)
    {
        std::cout << __FUNCTION__ << ": could not set value";
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    NodeFactory::ptr factory;
    factory.reset(new NodeFactory);
    internals::loadInternals(factory); // FIXME: depends on plugins-base
    if (! check_split_path())
        return 1;
    if (! check_dynamic_patching(factory))
        return 1;
    return 0;
}

