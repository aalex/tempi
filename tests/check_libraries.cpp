#include <iostream>
#include "tempi/nodefactory.h"
#include "tempi/internals.h"
#include "tempi/midi/midilibrary.h"

using namespace tempi;

bool check_midi_library()
{
    NodeFactory factory;
    midi::MidiLibrary lib;
    lib.load(factory, "midi.");

    if (! factory.hasType("midi.receive"))
    {
        std::cout << __FUNCTION__ << ": Did not register node successfully" << std::endl;
        return false;
    }
    return true;
}

static bool didRegister(NodeFactory &factory, const char *name)
{
    if (! factory.hasType(name))
    {
        std::cout << "Did not register node successfully " << name << std::endl;
        return false;
    }
    return true;
}

bool check_librarytools()
{
    NodeFactory factory;
    librarytools::loadLibrary<midi::MidiLibrary>(factory, "midi.");

    if (! didRegister(factory, "midi.receive")) return false;

    return true;
}

bool check_internals()
{
    NodeFactory factory;
    internals::loadInternals(factory);
    if (! didRegister(factory, "base.metro")) return false;
    if (! didRegister(factory, "base.nop")) return false;
    if (! didRegister(factory, "base.print")) return false;
    if (! didRegister(factory, "midi.receive")) return false;
    if (! didRegister(factory, "osc.receive")) return false;
    if (! didRegister(factory, "osc.send")) return false;
    if (! didRegister(factory, "sampler.sampler")) return false;
    return true;
}

int main(int argc, char *argv[])
{
    if (! check_midi_library()) return 1;
    if (! check_librarytools()) return 1;
    if (! check_internals()) return 1;
    return 0;
}

