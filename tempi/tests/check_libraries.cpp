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
    // FIXME: this is plugins-base specific. should not be in core lib tests
    if (! didRegister(factory, "base.time.metro")) return false;
    if (! didRegister(factory, "base.flow.nop")) return false;
    if (! didRegister(factory, "base.flow.print")) return false;
    if (! didRegister(factory, "base.midi.input")) return false;
    if (! didRegister(factory, "base.osc.receive")) return false;
    if (! didRegister(factory, "base.osc.send")) return false;
    if (! didRegister(factory, "base.sampler.simple")) return false;
    return true;
}

int main(int argc, char *argv[])
{
    if (! check_internals()) return 1;
    return 0;
}

