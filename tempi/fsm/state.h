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
 * The State class.
 */
#ifndef __TEMPI_STATE_H__
#define __TEMPI_STATE_H__

#include <boost/signals2.hpp>
#include <tr1/memory>
#include <string>

namespace tempi
{

namespace fsm
{

/**
 * State in a finite state machine.
 */
class State 
{
    public:
        typedef std::tr1::shared_ptr<State> ptr;
        boost::signals2::signal<void ()> on_entered_signal_;
        boost::signals2::signal<void ()> on_leaved_signal_;
        State(const std::string &name);
        const std::string &getName();
        void setName(const std::string &name);
        // TODO: add substates
    private:
        std::string name_;
};

} // end of namespace

} // end of namespace

#endif // ifndef

