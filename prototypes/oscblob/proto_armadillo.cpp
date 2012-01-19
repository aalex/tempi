//g++ example.cpp -o example -O1 -larmadillo
#include <iostream>
#include <armadillo>

int main(int argc, char* argv[])
{
    arma::mat a = arma::randu<arma::mat>(4, 5);
    arma::mat b = arma::randu<arma::mat>(4, 5);
  
    std::cout << a * arma::trans(b) << std::endl;
    return 0;
}

