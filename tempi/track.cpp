#include "tempi/track.h"
#include <boost/tuple/tuple_io.hpp>
#include <iostream>
#include <cmath> // for std::abs

namespace tempi
{

Track::Track()
{
    // pass
}

void Track::reset()
{
    events_.clear();
}

TimeStamp Track::getDuration()
{
    if (events_.size() == 0)
        return 0L;
    return ((events_[events_.size() - 1])).get<0>();
}

void Track::add(TimeStamp position, boost::any value)
{
    events_.insert(getIteratorAfter(position), Event(position, value));
    //std::cout << "add " << position << ": ?";
    //std::cout << " ... We have " << events_.size() << " events.";
    //std::cout << std::endl;
}

EventVecIter Track::getIteratorAfter(TimeStamp target)
{
    EventVecIter iter;
    EventVecIter ret;
    ret = events_.begin();
    TimeStamp smallest = getDuration();
    bool found_it = false;
    
    for (iter = events_.begin(); iter < events_.end(); ++iter)
    {
        TimeStamp pos = (*iter).get<0>();
        //if (target >= pos)
        if (pos >= target)
        {
            ret = iter;
            found_it = true;
            break;
        }
    }
    if (! found_it)
        ret = events_.end();
    return ret;
}

boost::any *Track::getClosest(TimeStamp target)
{
    TimeStamp duration = getDuration();
    if (numberOfEvents() == 0)
    {
        return 0;
    }
    else if (duration == 0L)
    {
        return getFirst();
    }
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
    return &(*ret).get<1>();
}

void Track::print()
{
    EventVecIter iter;
    for (iter = events_.begin(); iter < events_.end(); ++iter)
    {
        TimeStamp point = (*iter).get<0>();
        boost::any value = (*iter).get<1>();
        //std::cout << " * " << point << ": " << boost::any_cast<boost::tuple<double, double> >(value) << std::endl;
    }
}

unsigned int Track::numberOfEvents()
{
    return events_.size();
}

boost::any *Track::getFirst()
{
    return &(*events_.begin()).get<1>();
}

boost::any *Track::getLast()
{
    &events_[events_.size() - 1].get<1>();
}

} // end of namespace

