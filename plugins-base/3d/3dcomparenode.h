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

/**
 * @file
 * Math nodes that take two floats.
 */
#ifndef __TEMPI_3D_COMPARE_NODE_H__
#define __TEMPI_3D_COMPARE_NODE_H__

#include "tempi/config.h" // include before check for HAVE_SPATOSC

#ifdef HAVE_SPATOSC
#include "tempi/message.h"
#include <spatosc/maths.h>
#include "tempi/node.h"
#include "tempi/utils.h"

namespace tempi { 
namespace plugins_base { 

/**
 * Abstract3dCompareNode is an abstract class for nodes that accept two 3-float vectors.
 */
class Abstract3dCompareNode : public Node
{
    public:
        Abstract3dCompareNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
    private:
        virtual void compare(const spatosc::Vector3 &left, const spatosc::Vector3 &right) = 0;
        static const char * const HOT_INLET;
        static const char * const COLD_INLET;
        static const char * const OPERAND_ATTR;
};

/**
 * Ouputs the angle and elevation between two 3d-float messages.
 */
class Angle3dNode : public Abstract3dCompareNode
{
    public:
        Angle3dNode();
    private:
        virtual void compare(const spatosc::Vector3 &left, const spatosc::Vector3 &right);
        static const char * const ANGLE_ELEVATION_OUTLET;
};

} // end of namespace
} // end of namespace

#endif // HAVE_SPATOSC
#endif // ifndef

