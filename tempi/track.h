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

#ifndef __TEMPI_TRACK_H__
#define __TEMPI_TRACK_H__

#include <boost/signals2.hpp>
#include <tr1/memory>
#include <map>
#include <vector>
#include "tempi/keyframe.h"
#include "tempi/timeposition.h"
#include "tempi/identifier.h"

namespace tempi
{

/**
 * A Track contains the subsequent changes of a value over time.
 * Each value is in a KeyFrame.
 */
template <typename T>
class Track
{
    public:
        typedef std::tr1::shared_ptr<KeyFrame<T> > KeyFramePtr;
        Track(Identifier id) :
            id_(id)
        {}
        void setName(const std::string &name)
        {
            name_ = name;
        }
        const std::string &getName() const
        {
            return name_;
        }
        Identifier getIdentifier() const
        {
            return id_;
        }
        bool addKeyFrame(TimePosition position, KeyFramePtr frame)
        {
            keyFrames_[position] = frame;
            return true;
        }
        bool removeKeyFrame(KeyFrame<T> *frame)
        {
            typename std::map<TimePosition, KeyFramePtr>::iterator iter;
            for (iter = keyFrames_.begin(); iter != keyFrames_.end(); ++iter)
            {
                KeyFrame<T> *current = iter->second.get();
                if (current == frame)
                {
                    keyFrames_.erase(iter);
                    return true;
                }
            }
            return false;
        }
        void setMute(bool mute)
        {
            isMute_ = mute;
        }
        bool getMute() const
        {
            return isMute_;
        }
        std::vector<KeyFramePtr> getKeyFrames(TimePosition from, TimePosition to)
        {
            std::vector<KeyFramePtr> ret;
            typename std::map<TimePosition, KeyFramePtr>::iterator iter;
            for (iter = keyFrames_.begin(); iter != keyFrames_.end(); ++iter)
            {
                TimePosition pos = iter->first;
                if (from <= pos && pos <= to)
                {
                    ret.push_back(iter->second);
                }
            }
            return ret;
        }
        KeyFrame<T> *getKeyFrame(TimePosition pos)
        {
            if (keyFrames_.find(pos) != keyFrames_.end())
            {
                return keyFrames_.find(pos)->second.get();
            }
            return 0;
        }
        const std::string &getTypeName() const
        {
            return typeid(T).name();
        }
    private:
        std::map<TimePosition, KeyFramePtr> keyFrames_;
        bool isMute_;
        std::string name_;
        Identifier id_;
};

} // end of namespace

#endif // ifndef

