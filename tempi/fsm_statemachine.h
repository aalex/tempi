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
 * The StateMachine class.
 */
#ifndef __TEMPI_STATEMACHINE_H__
#define __TEMPI_STATEMACHINE_H__

#include <boost/signals2.hpp>
#include <map>
#include <string>
#include <tr1/memory>
#include <vector>
#include "tempi/fsm_state.h"
#include "tempi/fsm_rule.h"

namespace tempi {
namespace fsm {

/**
 * Finite state machine.
 */
class StateMachine 
{
    public:
        typedef std::tr1::shared_ptr<StateMachine> ptr;
        boost::signals2::signal<void (std::string)> on_state_changed_signal_;
        StateMachine();
        bool addRule(Rule *rule);
        bool addState(State *state);
        const std::string &getCurrentStateName();
        bool setCurrentStateName(const std::string &name);
        State *getState(const std::string &name);
        bool triggerEvent(const std::string &event);
        bool hasStateName(const std::string &name);
    private:
        std::map<std::string, State::ptr> states_;
        std::vector<Rule::ptr> rules_;
        std::string current_state_;
};

} // end of namespace
} // end of namespace

#endif // ifndef

