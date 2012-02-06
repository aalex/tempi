//g++ example.cpp -o example -O1 -larmadillo
#include <iostream>
#include <armadillo>

int main(int argc, char* argv[])
{
    arma::mat a = arma::zeros<arma::mat>(160, 120);
    arma::mat b = a.submat(80, 60, 90, 70); // x1, y1, x2, y2
  
    std::cout << b << std::endl;
    return 0;
}

