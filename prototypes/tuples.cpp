#include <typeinfo>
#include <boost/tuple/tuple.hpp>
#include <boost/any.hpp>
#include <iostream>

template <typename T>
unsigned int len()
{
    return boost::tuples::length<T>::value;
}

typedef boost::tuple<float, float> _ff;

int main(int argc, char *argv[])
{
    //boost::tuple<float, float> ff;
    boost::any data(_ff(3.14156, 2.0));
    unsigned int length = len<_ff>();
    std::cout << "Length: " << length << std::endl;
    std::cout << "0th type:: " << typeid(boost::tuples::element<0, _ff>::type).name() << std::endl;
    //std::cout << "Length: " << length << std::endl;
    return 0;
}
