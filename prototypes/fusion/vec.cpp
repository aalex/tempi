#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/at.hpp>
#include <iostream>

using namespace boost;

int main(int argc, char *argv[])
{
    fusion::vector<int, char, std::string> vec(1, 'x', "howdy");
    std::cout << "at 0: " << fusion::at_c<0>(vec) << std::endl;
    fusion::at_c<0>(vec) = 2;
    std::cout << "at 0: " << fusion::at_c<0>(vec) << std::endl;
    return 0;
}

