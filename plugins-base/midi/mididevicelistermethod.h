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

/**
 * @file
 * The MidiDeviceListerMethod class.
 */
#ifndef __TEMPI_MIDI_MIDIDEVICELISTERMETHOD_H__
#define __TEMPI_MIDI_MIDIDEVICELISTERMETHOD_H__

#include "tempi/midi.h"
#include "tempi/entitymethod.h"
#include <map>

namespace tempi {
namespace plugins_base {

/**
 * Lists MIDI devices.
 */
class MidiDeviceListerMethod : public EntityMethod
{
    public:
        typedef enum {
            SOURCE = 0, 
            DESTINATION = 1
        } Direction;

        MidiDeviceListerMethod(const char *name, const char *short_documentation,
            const char *long_documentation, Direction direction);
        ~MidiDeviceListerMethod()
        {
        }
    protected:
        virtual bool doCall(const Message &arguments, Message & return_value);
    private:
        Direction direction_;
        static std::map<int, std::string> list_input_midi_devices();
        static std::map<int, std::string> list_output_midi_devices();
};

} // end of namespace
} // end of namespace

#endif // ifndef


