/*
 * Copyright (C) 2011 Alexandre Quessy
 * 
 * This file is part of Tempi.
 * 
 * Tempi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

/**
 * @file
 * The Sink class.
 */
#ifndef __TEMPI_SINK_H__
#define __TEMPI_SINK_H__

#include <boost/signals2.hpp>
#include <vector>
#include "tempi/sharedptr.h"
#include "tempi/message.h"

namespace tempi
{

class Source; // forward declaration

/**
 * A Sink is a pad to which we can connect Source pads.
 */
class Sink
{
    public:
        Sink();
        bool connect(std::tr1::shared_ptr<Source> source);
        void onTriggered(Source *source, Message &message);
        boost::signals2::signal<void (Source *, Message&)> on_triggered_signal_;
    private:
        std::vector<std::tr1::shared_ptr<Source> > sources_;
        bool hasSource(Source *source);
};

} // end of namespace

#endif // ifndef

