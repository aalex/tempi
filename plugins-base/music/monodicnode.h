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
 * The MonodicNode class.
 */
#ifndef __TEMPI_MUSIC_MONODIC_NODE_H__
#define __TEMPI_MUSIC_MONODIC_NODE_H__

#include "tempi/node.h"

namespace tempi {
namespace plugins_base {

/**
 * Generates a stream of MIDI notes that are closed so that it's monophonic.
 */
class MonodicNode : public Node
{
    public:
        MonodicNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
    private:
        bool has_open_note_;
        float open_note_;
        void closeNote();
};

} // end of namespace
} // end of namespace

#endif // ifndef

