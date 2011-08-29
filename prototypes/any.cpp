#include <boost/any.hpp>
#include <boost/tuple/tuple.hpp>
#include <vector>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    std::vector<boost::any> vec;
    vec.push_back(2);
    vec.push_back(3.14159);
    vec.push_back(std::string("hello"));
    vec.push_back(boost::tuple<double, double>(3.14159, 1.6666));
    
    std::vector<boost::any>::iterator iter;
    for (iter = vec.begin(); iter != vec.end(); ++iter)
    {
        boost::any any = (*iter);
        if (any.type() == typeid(int))
        {
            std::cout << "(int) ";
            std::cout << boost::any_cast<int>(any);
        }
        else if (any.type() == typeid(double))
        {
            std::cout << "(double) ";
            std::cout << boost::any_cast<double>(any);
        }
        else if (any.type() == typeid(std::string))
        {
            std::cout << "(string) ";
            std::cout << boost::any_cast<std::string>(any);
        }
        else if (any.type() == typeid(boost::tuple<double, double>))
        {
            std::cout << "(double, double) ";
            boost::tuple<double, double> value = boost::any_cast<boost::tuple<double, double> >(any);
            std::cout << value.get<0>() << " " << value.get<1>();
        }
        std::cout << std::endl;
    }
    return 0;
}

