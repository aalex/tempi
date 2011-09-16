#include <iostream>
#include "tempi/tempi.h"
#include "tempi/slide.h"

static const bool VERBOSE = false;

using namespace tempi;

bool check_slide()
{
    Slide slide = Slide();
    slide.setSlide(1.0);
    std::cout << "slide = " << slide.getSlide() << std::endl;
    double factor = 1.0;
    for (int i = 0; i < 100; ++i)
    {
        double result = slide.slide(i * factor);
        if (VERBOSE)
            std::cout << "" << i << " = " << result << std::endl;
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (not check_slide())
        return 1;
    return 0;
}

