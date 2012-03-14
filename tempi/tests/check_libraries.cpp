#include <iostream>
#include "tempi/nodefactory.h"
#include "tempi/internals.h"

using namespace tempi;

static bool didRegister(NodeFactory &factory, const char *name)
{
    if (! factory.hasType(name))
    {
        std::cout << "Did not register node successfully " << name << std::endl;
        return false;
    }
    return true;
}

bool check_internals()
{
    NodeFactory factory;
    internals::loadInternals(factory);
    if (! didRegister(factory, "base.metro")) return false;
    if (! didRegister(factory, "base.nop")) return false;
    if (! didRegister(factory, "base.print")) return false;
    if (! didRegister(factory, "midi.input")) return false;
    if (! didRegister(factory, "osc.receive")) return false;
    if (! didRegister(factory, "osc.send")) return false;
    if (! didRegister(factory, "sampler.sampler")) return false;
    return true;
}

int main(int argc, char *argv[])
{
    if (! check_internals()) return 1;
    return 0;
}

