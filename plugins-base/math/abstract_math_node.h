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
 * @file AbstractMathNode
 * 
 */
#ifndef __TEMPI_MATH_ABSTRACT_MATH_NODE_H__
#define __TEMPI_MATH_ABSTRACT_MATH_NODE_H__

#include "tempi/message.h"
#include "tempi/node.h"
#include "tempi/utils.h"

namespace tempi { 
namespace plugins_base { 

/**
 * AbstractMathNode is an abstract class for math nodes.
 */
class AbstractMathNode : public Node
{
    public:
        AbstractMathNode();
        static const char * const IN_INLET;
        static const char * const OUT_OUTLET;
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
    private:
        virtual Float calculate(Float operand) = 0;
        virtual void processUnhandledMessage(const char *inlet, const Message &message) = 0;
};

} // end of namespace
} // end of namespace

#endif // ifndef

