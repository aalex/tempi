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
 * The MapperDevice class.
 */
#ifndef __TEMPI_MAPPER_MAPPERDEVICE_H__
#define __TEMPI_MAPPER_MAPPERDEVICE_H__

#include "tempi/config.h"
#ifdef HAVE_LIBMAPPER

#include "tempi/message.h"
#include "tempi/concurrentqueue.h"
#include <mapper/mapper.h>

namespace tempi {
namespace plugins_base {

/**
 * Libmapper device
 */
class MapperDevice
{
    public:
        MapperDevice(const char *name, unsigned int port)
        {
            name_ = std::string(name);
            port_ = port;
            start();
        }
        bool start()
        {
            if (! isRunning())
                dev_ = mdev_new(name_.c_str(), port_, 0);
            return true;
        }
        bool poll(std::vector<Message> &result)
        {
            if (! is_running())
                start();
            mdev_poll(dev_, 0);
            while (! queue_.empty())
            {
                Message popped_value;
                if (queue_.try_pop(popped_value))
                    result.push_back(popped_value);
            }
            return true;
        }
        bool isRunning()
        {
            return (dev != 0);
        }
        ~MapperDevice()
        {
            mdev_free(dev_);
        }
        bool addFloatInput(const char *name, int length=1, const char *unit="", float minimum=0, float maximum=0)
        {
            mdev_add_input(name, length, 'f', unit,
                minimum, maximum,
                MapperDevice::inputHandler,
                (void *) this);
        }
        bool addIntInput(const char *name, int length=1, const char *unit="", int minimum=0, int maximum=0)
        {
            mdev_add_input(name, length, 'i', unit,
                minimum, maximum,
                MapperDevice::inputHandler,
                (void *) this);
        }
    private:
        mapper_device dev_;
        ConcurrentQueue<Message> queue_

        static void inputHandler(
            mapper_signal msig,
            mapper_db_signal props,
            mapper_timetag_t *timetag,
            void *value)
        {
            Message message;
            // get name
            // see https://github.com/radarsat1/mapperRec/blob/master/backend_oscstreamdb.c
            char str[1024], *path = str;
            int num_chars = msig_full_name(msig, path, 1024);
            std::string name;
            if (num_chars != 0)
            {
                for (int i = 0; str[i] != 0 && i < 1024; i++)
                    name += str[i];
            }
            message.appendString(name.c_str());
            // value:
            mapper_db_signal mprop = msig_properties(msig);
            if (mprop->type == 'i')
            {
                for (i = 0; i < mprop->length; i++)
                {
                    message.appendInt(((int*) value)[i]);
                }
            }
            else if (mprop->type == 'f')
            {
                for (i = 0; i < mprop->length; i++)
                {
                    message.appendFloat(((float*) value)[i]);
                }
            }
            queue_.push(message);
        }
};

} // end of namespace
} // end of namespace

#endif // HAVE_LIBMAPPER
#endif // ifndef

