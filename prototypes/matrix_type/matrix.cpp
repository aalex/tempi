#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <iostream>

int main (int argc, char *argv[])
{
    using namespace boost::numeric::ublas;
    matrix<double> m(3, 3);
    m.insert_element(1, 0, 3.14159);
    std::cout << m << std::endl;
}

