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
#include "tempi/oscsender.h"
#include <sstream>
#include <string>

namespace tempi {
namespace osc {

OscSender::OscSender(const char *host, unsigned int port) :
    port_(port),
    host_(host)
{
    std::ostringstream os;
    os << port_;
    lo_address_ = lo_address_new(host_.c_str(), os.str().c_str());
}

OscSender::~OscSender()
{
    lo_address_free(lo_address_);
}

bool OscSender::sendMessage(const Message &message)
{
    std::string path;
    AtomType firstArgType;
    message.getAtomType(0, firstArgType);
    if (firstArgType == 's')
        path = message.getString(0);
    else
    {
        std::cerr << "OscSender::" << __FUNCTION__ << ": First arg must be an OSC path";
        return false;
    }
    if (! isValidPath(path))
    {
        std::cerr << "OscSender::" << __FUNCTION__ << ": Invalid OSC path";
        return false;
    }

    lo_message loMess = lo_message_new();
    for (unsigned int i = 1; i < message.getSize(); ++i)
    {
        AtomType type;
        message.getAtomType(i, type);
        switch (type)
        {
            case BOOLEAN:
            {
                bool value = message.getBoolean(i);
                if (value)
                    lo_message_add_true(loMess);
                else
                    lo_message_add_false(loMess);
                break;
            }
            case CHAR:
                lo_message_add_char(loMess, message.getChar(i));
                break;
            case DOUBLE:
                lo_message_add_double(loMess, message.getDouble(i));
                break;
            case FLOAT:
                lo_message_add_float(loMess, message.getFloat(i));
                break;
            case INT:
                lo_message_add_int32(loMess, message.getInt(i));
                break;
            case LONG:
                lo_message_add_int64(loMess, (int64_t) message.getLong(i));
                break;
            case STRING:
                lo_message_add_string(loMess, message.getString(i).c_str());
                break;
            defaut:
                std::cerr << "OscSender::" << __FUNCTION__ << ": Unsupported type." << std::endl;
                break;
        }
    }
    lo_send_message(lo_address_, path.c_str(), loMess);
    lo_message_free(loMess);
    return true;
}

bool OscSender::isValidPath(const std::string &path)
{
    //TODO
    return true;
}

} // end of namespace
} // end of namespace

