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

#ifndef __TEMPI_NODE_H__
#define __TEMPI_NODE_H__

#include <boost/any.hpp>
#include <vector>
#include "tempi/source.h"
#include "tempi/sink.h"

namespace tempi
{

/**
 * A Node is something that element that can be connected to and from other elements.
 */
class Node
{
    public:
        Node();
        std::vector<SourcePtr> getSources();
        std::vector<SinkPtr> getSinks();
        bool hasSink(Sink *sink);
        bool hasSource(Source *source);
        bool addSource(SourcePtr source);
        bool addSink(SinkPtr sink);
    private:
        std::vector<SourcePtr> sources_;
        std::vector<SinkPtr> sinks_;
        void onSinkTriggered(Source *source, boost::any data);
};

} // end of namespace

#endif // ifndef

