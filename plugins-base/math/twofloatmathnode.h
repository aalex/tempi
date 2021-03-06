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
#ifndef __TEMPI_TWO_FLOAT_MATH_NODE_H__
#define __TEMPI_TWO_FLOAT_MATH_NODE_H__

#include "plugins-base/math/abstract_math_node.h"
#include "tempi/message.h"
#include "tempi/node.h"
#include "tempi/utils.h"

namespace tempi { 
namespace plugins_base { 

/**
 * TwoFloatMathNode is an abstract class for nodes that accept two floats and output a float.
 */
class TwoFloatMathNode : public AbstractMathNode
{
    public:
        TwoFloatMathNode();
        static const char * const RIGHT_OPERAND_ATTR;
        static const char * const RIGHT_OPERAND_INLET;
    private:
        // inherited from AbstractMathNode
        virtual Float calculate(Float first);
        // New method
        virtual Float calculate(Float first, Float second) = 0;
        virtual void processUnhandledMessage(const char *inlet, const Message &message);
};

/**
 * The AddNode adds two floats together.
 */
class AddNode : public TwoFloatMathNode
{
    public:
        AddNode();
    protected:
        virtual Float calculate(Float first, Float second);
};

/**
 * The DivNode divides two floats.
 */
class DivNode : public TwoFloatMathNode
{
    public:
        DivNode();
    private:
        virtual Float calculate(Float first, Float second);
};


/**
 * The MultNode multiplies two floats.
 */
class MultNode : public TwoFloatMathNode
{
    public:
        MultNode();
    private:
        virtual Float calculate(Float first, Float second);
};

/**
 * The SubtractNode subtracts one float from another
 */
class SubtractNode : public TwoFloatMathNode
{
    public:
        SubtractNode();
    private:
        virtual Float calculate(Float first, Float second);
};

/**
 * The ModuloNode outputs the leftover of the 
 */
class ModuloNode : public TwoFloatMathNode
{
    public:
        ModuloNode();
    private:
        virtual Float calculate(Float first, Float second);
};

} // end of namespace
} // end of namespace

#endif // ifndef

