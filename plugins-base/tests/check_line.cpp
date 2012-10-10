#include <iostream>
#include <unistd.h>
#include <cmath>
#include "plugins-base/flow/linenode.h"

static const bool VERBOSE = true;

using namespace tempi;

bool float_is_about(float left, float right)
{
    static float PRECISION = 0.05;
    float delta = left - right;
    return (std::abs(delta) <= PRECISION);
}

bool check_line()
{
    // go from 0.0 to 1.0 in 0.100 s
    float target = 1.0f;
    float duration = 100.0f;
    plugins_base::Line line = plugins_base::Line();
    line.start(target, duration);
    std::cout << "target: " << line.getTarget() << ", duration: " << line.getDuration() << std::endl;
    if (VERBOSE)
    {
        std::cout << "line: [" << line.getTarget() <<
            " " << line.getDuration() << "<" <<
            std::endl;
    }
    if (line.getTarget() != target)
    {
        std::cout << "target expected=" << target <<
            " actual=" << line.getTarget() << std::endl;
        return false;
    }
    if (line.getDuration() != duration)
    {
        std::cout << "duration expected=" << duration <<
            " actual=" << line.getDuration() << std::endl;
        return false;
    }
    const int num_iter = 10;
    long interval_ms = ((long) duration) / num_iter;
    std::cout << "interval: " << interval_ms << std::endl;
    for (int i = 0; i < num_iter; ++i)
    {
        usleep(interval_ms * 1000);
        float result = line.calculateCurrent();
        if (VERBOSE)
            std::cout << "line: " << i << " = " << result << std::endl;
        float expected = ((i+1) / (float) num_iter) * target;
        std::cout << "expected: " << expected << std::endl;
        if (! float_is_about(result, expected))
        {
            std::cout << "result expected=" << expected <<
                " actual=" << result << std::endl;
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (! check_line())
        return 1;
    return 0;
}

