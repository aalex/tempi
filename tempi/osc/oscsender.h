
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
 * The OscSender class.
 */

#ifndef __OSC_SENDER_H__
#define __OSC_SENDER_H__

#include <iostream>
#include <string>
#include <lo/lo.h>
#include "tempi/message.h"

namespace tempi
{

/**
 * Sends OpenSoundControl messages.
 */
class OscSender
{
    public:
        OscSender(const char *host, unsigned int port);
        virtual ~OscSender();
        bool sendMessage(const Message &message);
    private:
        unsigned int port_;
        std::string host_;
        lo_address lo_address_;
};

} // end of namespace

#endif // __OSC_SENDER_H__

