/*
 * Copyright (C) 2011 Alexandre Quessy
 *
 * This file is part of Tempi.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software ither version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tempi.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 * The Pad class.
 */
#ifndef __TEMPI_PAD_H__
#define __TEMPI_PAD_H__

//#include <boost/signals2.hpp>
#include "tempi/entity.h"
#include "tempi/message.h"

namespace tempi {

/**
 * A Pad is something that can be connected to something else.
 */
class Pad : public Entity
{
    public:
        /**
         * First argument: the name of the pad?
         */
        typedef boost::signals2::signal<void (const char *, const Message&)> TriggeredSignal;
        Pad(const char *name,
            const char *short_documentation,
            const char *long_documentation);
        // TODO: return bool
        void trigger(const Message &message);
        TriggeredSignal &getOnTriggeredSignal();
        // TODO:
        // void setAcceptedTypes(const std::vector<std::string> &types_tags);
        // std::vector<std::string> getAcceptedTypes() const;
    protected:
        TriggeredSignal on_triggered_signal_;
};

} // end of namespace

#endif // ifndef


