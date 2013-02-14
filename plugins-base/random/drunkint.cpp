/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 * Copyright (C) 2012 Emmanuel Durand
 *
 * This file is part of Tempi-plugins-base.
 *
 * This program is free software; you can redistither version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Tempi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "plugins-base/random/drunkint.h"
#include "tempi/config.h"
#include <iostream>
#ifdef HAVE_GLIB
#include <glib.h>
#endif
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

DrunkInt::DrunkInt() :
    from_(0),
    to_(1),
    current_(0),
    min_step_(0),
    max_step_(1)
{
    // pass
}

bool DrunkInt::setRange(int from, int to)
{
    if (from <= to)
    {
        from_ = from;
        to_ = to;
        if (current_ < from_)
            current_ = from_;
        else if (current_ > to_)
            current_ = to_;
        return true;
    }
    else
    {
        std::ostringstream os;
        os << "DrunkInt: range from must be smaller or equal than range to.";
        tempi::Logger::log(tempi::ERROR, os);
        return false;
    }
}

bool DrunkInt::setStepRange(int min_step, int max_step)
{
    if (min_step <= max_step)
    {
        min_step_ = min_step;
        max_step_ = max_step;
        return true;
    }
    else
    {
        std::ostringstream os;
        os << "DrunkInt: step range from must be smaller or equal than step range to.";
        tempi::Logger::log(tempi::ERROR, os);
        return false;
    }
}

bool DrunkInt::setCurrent(int value)
{
    if (value >= from_ || value <= to_)
    {
        current_ = value;
        return true;
    }
    else
    {
        std::ostringstream os;
        os << "Trying to jump to value " << value << " outside of range [" << from_ << "," << to_ << "]";
        tempi::Logger::log(tempi::ERROR, os);
        return false;
    }
}

int DrunkInt::step()
{
#ifdef HAVE_GLIB
    // TODO: make drunk steps configurable
    gint32 step = g_random_int_range(min_step_, max_step_ + 1);
    if (g_random_int_range(0, 2) == 0) // direction
        step = -step; // backwards
    current_ += step;
    if (current_ < from_)
        current_ = from_;
    if (current_ > to_)
        current_ = to_;
    return current_;
#else
    std::ostringstream os;
    os << "DrunkInt: disabled since compiled without glib support.";
    tempi::Logger::log(tempi::ERROR, os);
    return current_;
#endif
}

int DrunkInt::getRangeFrom() const
{
    return from_;
}

int DrunkInt::getRangeTo() const
{
    return to_;
}

int DrunkInt::getCurrent() const
{
    return current_;
}

int DrunkInt::getStepRangeMin() const
{
    return min_step_;
}

int DrunkInt::getStepRangeMax() const
{
    return max_step_;
}

} // end of namespace
} // end of namespace

