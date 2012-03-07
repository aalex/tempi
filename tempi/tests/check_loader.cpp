#include "tempi/loader.h"
#include "tempi/nodefactory.h"

int main(int argc, char *argv[])
{
    std::cout << "create Loader" << std::endl;
    tempi::Loader loader = tempi::Loader::getInstance();
    loader.addPath(".");
    loader.addPath("/usr/local/lib/tempi-0.1");
    loader.addPath("/usr/lib/tempi-0.1");
    std::cout << "Loader::load(\"hello\")" << std::endl;
    tempi::NodeFactory factory;
    bool success = loader.load(factory, "hello");
    if (! success)
        return 1;
    return 0;
}

