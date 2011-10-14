#include <iostream>
#include "tempi/mapping/average.h"

using namespace tempi;

bool check_average()
{
    Average<double> a(3);
    a.add(1.0);
    a.add(2.0);
    a.add(3.0);
    double result = a.getAverage();
    if (result != 2.0)
    {
        std::cout << "Expected average of 2.0 but got " << result << std::endl;
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    if (not check_average())
        return 1;
    return 0;
}

