#include "sampler.h"
#include <iostream>
#include <cmath> // for std::abs

namespace tempi
{

Sampler::Sampler()
{
    // pass
}

void Sampler::reset()
{
    writer_timer_.reset();
    reader_timer_.reset();
    points_.clear();
}

TimePoint Sampler::getDuration()
{
    if (points_.size() == 0)
        return 0L;
    return ((points_[points_.size() - 1])).get<0>();
}

void Sampler::add(ff value)
{
    //std::cout << "add " << value.get<0>() << " " << value.get<1>() << std::endl;
    std::cout << "add " << value << std::endl;
    points_.push_back(Point(writer_timer_.elapsed(), value));
}

ff Sampler::readLoop()
{
    TimePoint duration = getDuration();
    TimePoint elapsed = reader_timer_.elapsed();
    //std::cout << __FUNCTION__ << std::endl;
    if (points_.size() == 0)
    {
        //std::cout << "No point to read." << std::endl;
        return ff(0.0, 0.0);
    }
    else if (duration == 0L)
    {
        //std::cout << "Read first point." << std::endl;
        return (*points_.begin()).get<1>();
    }
    else if (duration == elapsed)
    {
        //std::cout << "Read last point." << std::endl;
        return points_[points_.size() - 1].get<1>();
    }
    else
    {
        //std::cout << "duration: " << getDuration() << std::endl;
        // FIXME: using the modulo here should not be mandatory
        TimePoint cursor = elapsed % duration;
        //std::cout << "elapsed modulo duration: " << cursor << std::endl;
        return (*getClosest(cursor)).get<1>();
    }
}

PointVecIter Sampler::getClosest(TimePoint target)
{
    PointVecIter iter;
    PointVecIter ret;
    ret = points_.begin();
    TimePoint smallest = getDuration();
    
    for (iter = points_.begin(); iter < points_.end(); ++iter)
    {
        TimePoint pos = (*iter).get<0>();
        TimePoint distance = std::abs(target - pos);
        if (distance < smallest)
        {
            //std::cout << "smallest:" << smallest << " target=" << target << " pos=" << pos << " abs:" << distance << std::endl;
            ret = iter;
            smallest = distance;
        }
    }
    return ret;
}

void Sampler::print()
{
    PointVecIter iter;
    for (iter = points_.begin(); iter < points_.end(); ++iter)
    {
        TimePoint point = (*iter).get<0>();
        ff value = (*iter).get<1>();
        //std::cout << " * " << point << ": " << value << std::endl;
    }
}

} // end of namespace

