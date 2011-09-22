#include "tempi/property.h"
#include <string>
#include <map>
#include <iostream>

using namespace tempi;

static const bool VERBOSE = false;

static void cb(const Property &property)
{
    if (VERBOSE)
        std::cout << property.getName() << " changed to " << property.getValue_ff() << std::endl;
}

bool check_property()
{
    Property foo("foo", _ff(1.0, 2.0), "some description");
    foo.value_changed_signal_.connect(&cb);
    foo.setValue<_ff>(_ff(3.0, 4.0));

    return true;
}

int main(int /* argc */, char ** /*argv */)
{
    if (! check_property())
        return 1;

    return 0;
}

