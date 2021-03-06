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
 * The SamplerNode class.
 */

#ifndef __TEMPI_SAMPLERNODE_H__
#define __TEMPI_SAMPLERNODE_H__

#include <boost/signals2.hpp>
#include <tr1/memory>
#include "tempi/sampler_score.h"
#include "tempi/timeposition.h"
#include "tempi/identifier.h"
#include "tempi/sampler_region.h"
#include "tempi/sampler_recorder.h"
#include "tempi/sampler_player.h"
#include "tempi/node.h"
//#include "tempi/sampler/marker.h"

namespace tempi {
namespace plugins_base {

/**
 * A SamplerNode is a node with a Score in it.
 * TODO
 */
class SamplerNode : public Node
{
    public:
        SamplerNode();
        // TODO:
        // bool addTrack(int number);
        // bool removeTrack(int number);
        // bool muteTrack(int number);
        // bool soloTrack(int number);
        // Track* getTrack(int number);
        // bool write(boost::any data);
        // boost::any data read();
        // TimePosition getDuration ();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        virtual bool onNodeAttributeChanged(const char *name, const Message &value);
    private:
//        bool recording_;
//        bool playing_;
        sampler::Region::ptr region_;
        sampler::Recorder::ptr recorder_;
        sampler::Player::ptr player_;

        void play(bool enabled);
        void record(bool enabled);
        virtual void doTick();

        // TODO:
        //std::map<Identifier, TrackPtr> tracks_;
        //std::map<TimePosition, MarkerPtr> markers_;
};

} // end of namespace
} // end of namespace

#endif // ifndef

