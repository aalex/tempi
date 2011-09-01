#include "sampler.h"
#include <boost/tuple/tuple_io.hpp>
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
    events_.clear();
}

TimeStamp Sampler::getDuration()
{
    if (events_.size() == 0)
        return 0L;
    return ((events_[events_.size() - 1])).get<0>();
}

void Sampler::add(boost::any value)
{
    //std::cout << "add " << value.get<0>() << " " << value.get<1>() << std::endl;
    //std::cout << "add " << value << std::endl;
    events_.push_back(Event(writer_timer_.elapsed(), value));
}

boost::any *Sampler::readLoop()
{
    TimeStamp duration = getDuration();
    TimeStamp elapsed = reader_timer_.elapsed();
    //std::cout << __FUNCTION__ << std::endl;
    if (events_.size() == 0)
    {
        //std::cout << "No point to read." << std::endl;
        return 0;
    }
    else if (duration == 0L)
    {
        //std::cout << "Read first point." << std::endl;
        return &(*events_.begin()).get<1>();
    }
    else if (duration == elapsed)
    {
        //std::cout << "Read last point." << std::endl;
        return &events_[events_.size() - 1].get<1>();
    }
    else
    {
        //std::cout << "duration: " << getDuration() << std::endl;
        // FIXME: using the modulo here should not be mandatory
        TimeStamp cursor = elapsed % duration;
        //std::cout << "elapsed modulo duration: " << cursor << std::endl;
        return &(*getClosest(cursor)).get<1>();
    }
}

EventVecIter Sampler::getClosest(TimeStamp target)
{
    EventVecIter iter;
    EventVecIter ret;
    ret = events_.begin();
    TimeStamp smallest = getDuration();
    
    for (iter = events_.begin(); iter < events_.end(); ++iter)
    {
        TimeStamp pos = (*iter).get<0>();
        TimeStamp distance = std::abs(target - pos);
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
    EventVecIter iter;
    for (iter = events_.begin(); iter < events_.end(); ++iter)
    {
        TimeStamp point = (*iter).get<0>();
        boost::any value = (*iter).get<1>();
        //std::cout << " * " << point << ": " << value << std::endl;
    }
}

} // end of namespace

