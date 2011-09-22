#include <iostream>
#include <string>
#include "tempi/message.h"

using namespace tempi;

static const bool VERBOSE = true;

bool check_message()
{
    Message m;
    m.appendFloat(3.14159);
    m.appendString(std::string("hello"));
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

int main(int argc, char *argv[])
{
    if (not check_message())
        return 1;
    return 0;
}

