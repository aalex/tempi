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
 * Math nodes that take one floats and output one float.
 */
#ifndef __TEMPI_ONE_FLOAT_MATH_NODE_H__
#define __TEMPI_ONE_FLOAT_MATH_NODE_H__

#include "tempi/message.h"
#include "plugins-base/math/abstract_math_node.h"
#include "tempi/node.h"
#include "tempi/utils.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace tempi { 
namespace plugins_base { 

/**
 * OneFloatMathNode is an abstract class for nodes that accept one float and output a float.
 */
class OneFloatMathNode : public AbstractMathNode
{
    public:
        OneFloatMathNode();
    private:
        virtual void processUnhandledMessage(const char *inlet, const Message &message) {}
};

/**
 * Converts degress to radians.
 */
class DegToRadNode : public OneFloatMathNode
{
    public:
        DegToRadNode();
    private:
        virtual Float calculate(Float operand);
};

/**
 * Converts a float from a range to another.
 */
class MapNode : public OneFloatMathNode
{
    public:
        MapNode();
    private:
        virtual Float calculate(Float operand);
};

/**
 * Outputs the sin of the input
 */
class SinNode : public OneFloatMathNode
{
    public:
        SinNode();
    private:
        virtual Float calculate(Float operand);
};

/**
 * outputs the cos of the input
 */
class CosNode : public OneFloatMathNode
{
    public:
        CosNode();
    private:
        virtual Float calculate(Float operand);
};

/**
 * rounds float to a given precision
 */
class RoundNode : public OneFloatMathNode
{
    public:
        RoundNode();
    private:
        virtual Float calculate(Float operand);
};

} // end of namespace
} // end of namespace

#endif // ifndef

