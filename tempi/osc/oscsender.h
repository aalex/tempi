
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

#ifndef __TEMPI_OSCSENDER_H__
#define __TEMPI_OSCSENDER_H__

#include <iostream>
#include <string>
#include <lo/lo.h>
#include "tempi/message.h"
#include "tempi/sharedptr.h"

namespace tempi {
namespace osc {

/**
 * Sends OpenSoundControl messages.
 */
class OscSender
{
    public:
        typedef std::tr1::shared_ptr<OscSender> ptr;
        OscSender(const char *host, unsigned int port);
        virtual ~OscSender();
        bool sendMessage(const Message &message);
        //TODO
        static bool isValidPath(const std::string &path);
    private:
        unsigned int port_;
        std::string host_;
        lo_address lo_address_;
};

} // end of namespace
} // end of namespace

#endif // __OSC_SENDER_H__

