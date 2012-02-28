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
 * Math nodes that take one floats and output one float.
 */
#ifndef __TEMPI_ONE_FLOAT_MATH_NODE_H__
#define __TEMPI_ONE_FLOAT_MATH_NODE_H__

#include "tempi/message.h"
#include "tempi/node.h"
#include "tempi/utils.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace tempi { 

/**
 * @namespace tempi::math
 *
 * Math Tempi node types.
 */

namespace math {

/**
 * OneFloatMathNode is an abstract class for nodes that accept one float and output a float.
 */
class OneFloatMathNode : public Node
{
    public:
        OneFloatMathNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
    private:
        virtual float calculate(float operand) = 0;
};

/**
 * Converts degress to radians.
 */
class DegToRadNode : public OneFloatMathNode
{
    public:
        DegToRadNode();
    private:
        virtual float calculate(float operand);
};

} // end of namespace
} // end of namespace

#endif // ifndef

