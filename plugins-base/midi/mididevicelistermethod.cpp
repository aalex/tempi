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

#include "plugins-base/midi/mididevicelistermethod.h"
#include "tempi/utils.h"
#include "tempi/log.h"
#include <iostream>

namespace tempi {
namespace plugins_base {

MidiDeviceListerMethod::MidiDeviceListerMethod(const char *name, const char *short_documentation,
            const char *long_documentation, Direction direction) :
    EntityMethod(name, short_documentation, long_documentation, "*", "*"), 
    direction_(direction)
{
}

std::map<int, std::string> MidiDeviceListerMethod::list_input_midi_devices()
{
    std::map<int, std::string> devices = tempi::midi::Midi::listDevices(tempi::midi::Midi::SOURCE);
    return devices;
}

std::map<int, std::string> MidiDeviceListerMethod::list_output_midi_devices()
{
    std::map<int, std::string> devices = tempi::midi::Midi::listDevices(tempi::midi::Midi::DESTINATION);
    return devices;
}

bool MidiDeviceListerMethod::doCall(const Message &arguments, Message & return_value)
{
    std::map<int, std::string> devices;
    if (this->direction_ == SOURCE)
         devices = list_input_midi_devices();
    else
         devices = list_output_midi_devices();

    std::map<int, std::string>::const_iterator iter;
    for (iter = devices.begin(); iter != devices.end(); ++iter)
    {
        return_value.appendInt((*iter).first);
        return_value.appendString((*iter).second.c_str());

        if (Logger::isEnabledFor(INFO))
        {
            std::ostringstream os;
            os << "MidiDeviceListerMethod::doCall: Detect MIDI device " << (*iter).first << " : " << (*iter).second;
            Logger::log(INFO, os);
        }
    }
    return true;
}

} // end of namespace
} // end of namespace

