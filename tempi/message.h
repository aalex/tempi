 /*
 * Copyright (C) 2011 Alexandre Quessy
 * 
 * This file is part of Tempi.
 * 
 * Tempi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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
 * The Message class
 */

#ifndef __TEMPI_MESSAGE_H__
#define __TEMPI_MESSAGE_H__

#include <boost/any.hpp>
#include <vector>
#include <typeinfo>

namespace tempi
{

class Message
{
    public:
        /**
         * Constructor with no argument.
         */
        Message();
        Message(std::vector<boost::any> arguments);
        std::vector<boost::any> &getArguments();
        //std::vector<std::type_info&> &getTypes();
        const std::type_info *getType(unsigned int index) const;
        bool setArgument(unsigned int index, boost::any &value);
        boost::any *getArgument(unsigned int index);
        unsigned int getSize() const;
        bool append(boost::any &value);
    private:
        std::vector<boost::any> arguments_;
};

} // end of namespace

#endif // ifndef

