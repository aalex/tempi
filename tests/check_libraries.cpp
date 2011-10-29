#include <iostream>
#include "tempi/nodefactory.h"
#include "tempi/midi/midilibrary.h"

using namespace tempi;

bool check_midi_library()
{
    NodeFactory factory;
    midi::MidiLibrary lib;
    lib.load(factory, "midi.");

    if (! factory.hasType("midi.midiin"))
    {
        std::cout << __FUNCTION__ << ": Did not register node successfully" << std::endl;
        return false;
    }
    return true;
}

bool check_librarytools()
{
    NodeFactory factory;
    librarytools::loadLibrary<midi::MidiLibrary>(factory, "midi.");
    if (! factory.hasType("midi.midiin"))
    {
        std::cout << __FUNCTION__ << ": Did not register node successfully" << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (! check_midi_library())
        return 1;
    if (! check_librarytools())
        return 1;
    return 0;
}

