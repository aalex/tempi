#include <iostream>
#include "tempi/tempi-version.h"

namespace tempi
{

void hello()
{
    std::cout << "Hello from Tempi." << std::endl;
}

std::string get_version()
{
    return std::string(TEMPI_VERSION_S);
}

}; // end of namespace

