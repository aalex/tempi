/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 *
 * This file is part of Tempi.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of, either version 3 of the License, or
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
 * The OscSenderNode class.
 */
#ifndef __TEMPI_OSC_OSCSENDERNODE_H__
#define __TEMPI_OSC_OSCSENDERNODE_H__

#include "tempi/oscsender.h"
#include "tempi/node.h"

namespace tempi {
namespace plugins_base {

/**
 * Node that sends OSC messages.
 */
class OscSenderNode : public Node
{
    public:
        OscSenderNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        virtual void onNodeAttibuteChanged(const char *name, const Message &value);
    private:
        osc::OscSender::ptr osc_sender_;
};

} // end of namespace
} // end of namespace

#endif // ifndef

