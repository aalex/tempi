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
#ifndef __TEMPI_MATH_BOOLEAN_NODE_H__
#define __TEMPI_MATH_BOOLEAN_NODE_H__

#include "tempi/message.h"
#include "tempi/node.h"

namespace tempi { 
namespace plugins_base { 

/**
 * BooleanMathNode is an abstract class for nodes that accept two floats and output a boolean.
 */
class BooleanMathNode : public Node
{
    public:
        BooleanMathNode();
        static const char * const HOT_NUMBER_INLET;
        static const char * const COLD_NUMBER_INLET;
        static const char * const BOOL_OUTLET;
        static const char * const OPERAND_ATTR;
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
 * The IsGreaterOrEqualNode checks if one float is greater or equal than another. 
 */
class IsGreaterOrEqualNode : public BooleanMathNode
{
    public:
        IsGreaterOrEqualNode();
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

/**
 * The IsLessNode checks if one float is lesser or equal than another. 
 */
class IsLessOrEqualNode : public BooleanMathNode
{
    public:
        IsLessOrEqualNode();
    private:
        virtual bool compare(float first, float second);
};

} // end of namespace
} // end of namespace

#endif // ifndef

