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
 * Math nodes that take two floats.
 */
#ifndef __TEMPI_TWO_FLOAT_MATH_NODE_H__
#define __TEMPI_TWO_FLOAT_MATH_NODE_H__

#include "tempi/message.h"
#include "tempi/node.h"
#include "tempi/utils.h"

namespace tempi { 
namespace plugins_base { 

/**
 * TwoFloatMathNode is an abstract class for nodes that accept two floats and output a float.
 */
class TwoFloatMathNode : public Node
{
    public:
        TwoFloatMathNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
    private:
        virtual float calculate(float first, float second) = 0;
};

/**
 * The AddNode adds two floats together.
 */
class AddNode : public TwoFloatMathNode
{
    public:
        AddNode();
    protected:
        virtual float calculate(float first, float second);
};

/**
 * The DivNode divides two floats.
 */
class DivNode : public TwoFloatMathNode
{
    public:
        DivNode();
    private:
        virtual float calculate(float first, float second);
};


/**
 * The MultNode multiplies two floats.
 */
class MultNode : public TwoFloatMathNode
{
    public:
        MultNode();
    private:
        virtual float calculate(float first, float second);
};

/**
 * The SubtractNode subtracts one float from another
 */
class SubtractNode : public TwoFloatMathNode
{
    public:
        SubtractNode();
    private:
        virtual float calculate(float first, float second);
};

} // end of namespace
} // end of namespace

#endif // ifndef

