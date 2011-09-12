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

#ifndef __TEMPI_FILTER_H__
#define __TEMPI_FILTER_H__

#include <boost/any.hpp>
#include "tempi/node.h"
#include "tempi/source.h"
#include "tempi/sink.h"

namespace tempi
{

/**
 * A Filter is Node who has a sink and source and usually filters data.
 */
class Filter : public Node
{
    public:
        Filter();
        Source *getSource();
        Sink *getSink();
    private:
        virtual void processTrigger(Source *source, boost::any data);
        virtual boost::any filter(Source *source, boost::any data) = 0;
};

} // end of namespace

#endif // ifndef

