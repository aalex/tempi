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
 * The TablePlayer class.
 */
#ifndef __TEMPI_TABLE_PLAYER_H__
#define __TEMPI_TABLE_PLAYER_H__

#include "tempi/table/table.h"
#include "tempi/message.h"

namespace tempi {
namespace table {

/**
 * A TablePlayer plays events from a Table.
 */
class TablePlayer
{
    public:
        TablePlayer(Table *table);
        void setTable(Table *table);
        /**
         * Returns the Table played by this TablePlayer.
         */
        Table *getTable() const;
        bool setIndex(unsigned int index);
        unsigned int getIndex() const;
        /**
         * Changes the speed of this TablePlayer. Default is 1.
         */
        void setSpeed(unsigned int factor);
        unsigned int getSpeed() const;
        /**
         * Reads an event from the table at the index pointed to by the internal index and increment it afterwhile.
         * Returns 0 if none is found.
         * Never free this pointer.
         */
        bool read(Message &message);
        // TODO: add a signal
        // TODO void setPlaybackMode(TablePlayback *playback);
        // TODO TablePlayback *getPlaybackMode();
    private:
        Table *table_;
        unsigned int speed_;
        unsigned int index_;
};

} // end of namespace
} // end of namespace

#endif // ifndef

