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

#ifndef __TEMPI_SAMPLERNODE_H__
#define __TEMPI_SAMPLERNODE_H__

#include <boost/signals2.hpp>
#include <tr1/memory>
#include <map>
#include "tempi/score.h"
#include "tempi/timeposition.h"
#include "tempi/identifier.h"
#include "tempi/track.h"
#include "tempi/marker.h"

namespace tempi
{

/**
 * A SamplerNode is a node with a Score in it.
 */

class SamplerNode
{
    public:
        SamplerNode();

        // TODO:
        //bool addTrack(int number);
        //bool removeTrack(int number);
        //bool muteTrack(int number);
        //bool soloTrack(int number);

        Track* getTrack(int number);
        bool write(boost::any data);
        boost::any data read();

        // TODO: TimePosition getDuration ();
    private:
        tempi::Track sampler_;
        std::tr1::shared_ptr<tempi::Recorder> recorder_;
        std::tr1::shared_ptr<tempi::Player> player_;
        bool recording_;

        // TODO:
        //std::map<Identifier, TrackPtr> tracks_;
        //std::map<TimePosition, MarkerPtr> markers_;
};
class Score
{
    public:
        typedef std::tr1::shared_ptr<Track> TrackPtr;
        typedef std::tr1::shared_ptr<Marker> MarkerPtr;
        Score();

        bool addMarker(TimePosition timePosition, Identifier identifier);
        Marker *getMarker(Identifier identifier);
        std::map<TimePosition, MarkerPtr> getMarkers();
        bool removeMarker(Identifier identifier);

        bool addTrack<typename T> (Identifier identifier);
        Track* getTrack(Identifier identifier);
        bool removeTrack(Identifier identifier);
        std::map<Identifier, TrackPtr> getTracks();
        //no need: muteTrack(Identifier identifier);
        bool soloTrack(Identifier identifier);
        // TODO: TimePosition getDuration ();
    private:
        std::map<Identifier, TrackPtr> tracks_;
        std::map<TimePosition, MarkerPtr> markers_;
};

} // end of namespace

#endif // ifndef

