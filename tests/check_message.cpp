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
        float f;
        m.getFloat(0, f);
        std::cout << "float: " << f << std::endl;
        int i;
        m.getInt(1, i);
        std::cout << "int: " << i << std::endl;
        std::string s;
        m.getString(2, s);
        std::cout << "string: " << s << std::endl;
        // operator<<:
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

