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
#ifndef __TEMPI_MATH_BOOLEAN_NODE_H__
#define __TEMPI_MATH_BOOLEAN_NODE_H__

#include "tempi/message.h"
#include "tempi/node.h"

namespace tempi { 
namespace math {

/**
 * BooleanMathNode is an abstract class for nodes that accept two floats and output a boolean.
 */
class BooleanMathNode : public Node
{
    public:
        BooleanMathNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
    private:
        virtual bool compare(float first, float second) = 0;
};

/**
 * The EqualsNotNode checks if two floats are not equal
 */
class EqualsNotNode : public BooleanMathNode
{
    public:
        EqualsNotNode();
    private:
        virtual bool compare(float first, float second);
};

/**
 * The IsGreaterNode checks if one float is greater than another. 
 */
class IsGreaterNode : public BooleanMathNode
{
    public:
        IsGreaterNode();
    private:
        virtual bool compare(float first, float second);
};

/**
 * The IsEqualNode checks if 2 floats have the same value.
 */
class IsEqualNode : public BooleanMathNode
{
    public:
        IsEqualNode();
    private:
        virtual bool compare(float first, float second);
};

/**
 * The IsLessNode checks if one float is lesser than another. 
 */
class IsLessNode : public BooleanMathNode
{
    public:
        IsLessNode();
    private:
        virtual bool compare(float first, float second);
};


} // end of namespace
} // end of namespace

#endif // ifndef
