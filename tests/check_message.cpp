#include <iostream>
#include <string>
#include "tempi/message.h"

using namespace tempi;

static const bool VERBOSE = true;

bool check_message()
{
    Message m;
    m.appendAny(3.14159);
    m.appendAny(std::string("hello"));
    m.appendAny(2);

    if (VERBOSE)
    {
        std::cout << boost::any_cast<double>(*m.getArgument(0));
        std::cout << boost::any_cast<std::string>(*m.getArgument(1));
        std::cout << boost::any_cast<int>(*m.getArgument(2));
    }

//    if (result != 2.0)
//    {
//        std::cout << "Expected average of 2.0 but got " << result << std::endl;
//        return false;
//    }

    return true;
}

int main(int argc, char *argv[])
{
    if (not check_message())
        return 1;
    return 0;
}

