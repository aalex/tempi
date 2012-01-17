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
 * The MathLibrary class.
 */
#ifndef __TEMPI_MATHLIBRARY_H__
#define __TEMPI_MATHLIBRARY_H__

#include "tempi/library.h"
#include "tempi/message.h"
#include "tempi/node.h"
#include <iostream>

namespace tempi { 

/**
 * @namespace tempi::math
 *
 * Math Tempi node types.
 */

namespace math {

/**
 * The MathLibrary library contains objects to do some maths.
 */
class MathLibrary : public Library
{
    public:
        virtual void load(NodeFactory &factory, const char *prefix) const;
};

/**
 * The AddNode adds two floats together.
 */
class AddNode : public Node
{
    public:
        AddNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
};

/**
 * The DivNode divides two floats.
 */
class DivNode : public Node
{
    public:
        DivNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
};

/**
 * The EqualsNotNode checks if two floats are not equal
 */
class EqualsNotNode : public Node
{
    public:
        EqualsNotNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
};

/**
 * The IsGreaterNode checks if one float is greater than another. 
 */
class IsGreaterNode : public Node
{
    public:
        IsGreaterNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
};

/**
 * The IsEqualNode checks if 2 floats have the same value.
 */
class IsEqualNode : public Node
{
    public:
        IsEqualNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
};

/**
 * The IsLessNode checks if one float is lesser than another. 
 */
class IsLessNode : public Node
{
    public:
        IsLessNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
};

/**
 * The MultNode multiplies two floats.
 */
class MultNode : public Node
{
    public:
        MultNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
};

/**
 * The SubtractNode subtracts one float from another
 */
class SubtractNode : public Node
{
    public:
        SubtractNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
};

} // end of namespace
} // end of namespace

#endif // ifndef

