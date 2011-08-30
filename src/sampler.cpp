#include "sampler.h"
#include <iostream>

namespace tempi
{

Sampler::Sampler()
{
    // pass
}

void Sampler::reset()
{
    timer_.reset();
    points_.clear();
}

TimePoint Sampler::getDuration()
{
    if (points_.size() == 0)
        return 0L;
    return ((points_[points_.size() - 1])).get<0>();
}

void Sampler::add(double x, double y)
{
    points_.push_back(Point(timer_.elapsed(), ff(x, y)));
}

ff Sampler::readLoop()
{
    if (points_.size() == 0)
    {
        std::cout << "No point to read." << std::endl;
        return ff(0.0, 0.0);
    }
    else
    {
        std::cout << "duration: " << getDuration() << std::endl;
        TimePoint cursor = timer_.elapsed() % getDuration();
        std::cout << "elapsed modulo duration: " << cursor << std::endl;
        return (*getClosest(cursor)).get<1>();
    }
}

PointVecIter Sampler::getClosest(TimePoint point)
{
    PointVecIter iter;
    PointVecIter ret;
    ret = points_.begin();
    TimePoint smallest = getDuration();
    
    for (iter = points_.begin(); iter < points_.end(); ++iter)
    {
        TimePoint current = (*iter).get<0>();
        if (point - current < smallest)
        {
            ret = iter;
            smallest = current;
        }
    }
    return ret;
}

void Sampler::print()
{
    PointVecIter iter;
    for (iter = points_.begin(); iter < points_.end(); ++iter)
    {
        std::cout << " * " << (*iter).get<0>() << " " << 
            (*iter).get<1>().get<0>() << " " << 
            (*iter).get<1>().get<1>() << 
            std::endl;
    }
}

} // end of namespace
