#include <iostream>
extern "C"
void tempi_hello_setup()
{
    std::cout << "Hello from " << __FILE__ << " " <<  __FUNCTION__ << std::endl;
}

