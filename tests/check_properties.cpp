#include "tempi/property.h"
#include <string>
#include <iostream>

using namespace tempi;

static const bool VERBOSE = true;
static bool GLOBAL_check_property_success;

static void cb(Property &property)
{
    if (VERBOSE)
    {
        std::cout << property.getName() << ": " << property.getDescription() << std::endl;
        try
        {
            std::cout << property.getName() << " changed to " << property.getValue<_ff>() << std::endl;
        }
        catch (const BadArgumentTypeException &e)
        {
            std::cout << "Error: " << e.what() << std::endl;
            GLOBAL_check_property_success = false;
        }
    }
}

bool check_property()
{
    Property foo("foo", _ff(3.14159, 0.12345678910111213), "Some description.");
    foo.getOnChangedSignal().connect(&cb);
    foo.setValue<_ff>(_ff(2.7182818284, 1.6180339887));
    try
    {
        foo.setValue<_fff>(_fff(1.0f, 2.0f, 3.0f));
        std::cout << "Changing to a different type should have thrown an exception." << std::endl;
        return false;
    }
    catch (const BadArgumentTypeException &e)
    {
        // pass
    }

    return GLOBAL_check_property_success;
}

int main(int /* argc */, char ** /*argv */)
{
    GLOBAL_check_property_success = true;
    if (! check_property())
        return 1;

    return 0;
}

