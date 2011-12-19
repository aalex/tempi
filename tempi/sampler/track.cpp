#include "tempi/sampler/track.h"
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

TimePosition Track::getDuration()
{
    if (events_.size() == 0)
        return 0L;
    return ((events_[events_.size() - 1])).get<0>();
}

void Track::add(TimePosition position, Message value)
{
    events_.insert(getIteratorAfter(position), Event(position, value));
}

Track::EventVecIter Track::getIteratorAfter(TimePosition target)
{
    EventVecIter iter;
    EventVecIter ret;
    ret = events_.begin();
    TimePosition smallest = getDuration();
    bool found_it = false;
    
    for (iter = events_.begin(); iter < events_.end(); ++iter)
    {
        TimePosition pos = (*iter).get<0>();
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

Message *Track::getClosest(TimePosition target)
{
    TimePosition duration = getDuration();
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
    TimePosition smallest = getDuration();
    
    for (iter = events_.begin(); iter < events_.end(); ++iter)
    {
        TimePosition pos = (*iter).get<0>();
        TimePosition distance = (TimePosition) std::abs(((double) target) - ((double) pos));
        if (distance < smallest)
        {
            //std::cout << "smallest:" << smallest << " target=" << target << " pos=" << pos << " abs:" << distance << std::endl;
            ret = iter;
            smallest = distance;
        }
    }
    return &(*ret).get<1>();
}

bool Track::getClosestBefore(TimePosition target, TimePosition &resultPosition, Message **resultMessage)
{
    if (numberOfEvents() == 0)
        return false;
    else if (getDuration() == 0L)
        return false;

    EventVecIter iter;
    EventVecIter cursor = events_.begin();
    TimePosition smallest = getDuration();
    for (iter = events_.begin(); iter < events_.end(); ++iter)
    {
        TimePosition pos = (*iter).get<0>();
        if (pos <= target)
        {
            TimePosition distance = (TimePosition) std::abs(((double) target) - ((double) pos));
            if (distance < smallest)
            {
                cursor = iter;
                smallest = distance;
            }
        }
    }
    (* resultMessage) = &((*cursor).get<1>());
    (resultPosition) = (*cursor).get<0>();
    return true;
}

//void Track::print()
//{
//    // TODO
//    EventVecIter iter;
//    for (iter = events_.begin(); iter < events_.end(); ++iter)
//    {
//        //TimePosition point = (*iter).get<0>();
//        //Message value = (*iter).get<1>();
//        //std::cout << " * " << point << ": " << Message_cast<boost::tuple<double, double> >(value) << std::endl;
//    }
//}

unsigned int Track::numberOfEvents()
{
    return events_.size();
}

Message *Track::getFirst()
{
    return &(*events_.begin()).get<1>();
}

Message *Track::getLast()
{
    &events_[events_.size() - 1].get<1>();
}

} // end of namespace

