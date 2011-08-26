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

#ifndef __TEMPI_KEYFRAME_H__
#define __TEMPI_KEYFRAME_H__

#include <boost/signals2.hpp>

namespace tempi
{

/**
 * A KeyFrame contains the value for a Track at a given time.
 */
template <typename T>
class KeyFrame
{
    public:
        /**
         * Typedef for the on_entered_signal_ signal.
         */
        typedef boost::signals2::signal<void ()> OnEntered;

        /**
         * Typedef for the on_value_changed_ signal.
         */
        typedef boost::signals2::signal<void (T)> OnValueChanged;

        /**
         * Signals triggered when the value is changed.
         */
        OnValueChanged on_value_changed_;

        /**
         * Signals triggered when this frame is triggered.
         */
        OnEntered on_frame_triggered_;

        /**
         * Constructor.
         */
        KeyFrame(T value) :
            value_(value)
        {}

        /**
         * Returns the value of this frame.
         */
        T getValue() const { return value_; }

        /**
         * Sets the value of this frame.
         */
        void setValue(T value)
        {
            value_ = value;
            on_value_changed_(value_);
        }
    private:
        T value_;

};

} // end of namespace

#endif // ifndef

