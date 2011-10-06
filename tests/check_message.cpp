#include <iostream>
#include <string>
#include "tempi/message.h"

using namespace tempi;

static const bool VERBOSE = true;

bool check_message()
{
    Message m;
    m.appendFloat(3.14159);
    m.appendString("hello");
    m.appendInt(2);

    if (VERBOSE)
    {
        std::cout << "float: " << m.getFloat(0) << std::endl;
        std::cout << "string: " << m.getString(1) << std::endl;
        std::cout << "int: " << m.getInt(2) << std::endl;
        std::cout << "Message: " << m << std::endl;
    }
    return true;
}

bool check_clone()
{
    Message m;
    m.appendFloat(3.14159);
    m.appendString("hello");
    m.appendInt(2);
    Message x = m.cloneRange(1, 2);
    if (x.getSize() != 2)
    {
        std::cout << "Expected message of size 2 but got something else\n";
        return false;
    }
    if (x.getTypes() != "si")
    {
        std::cout << "Expected message with si types\n";
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (not check_message())
        return 1;
    return 0;
}

