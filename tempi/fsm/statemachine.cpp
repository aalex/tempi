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

#include "tempi/fsm/statemachine.h"
#include <iostream>

namespace tempi
{

namespace fsm
{

StateMachine::StateMachine()
{
    // pass
}

bool StateMachine::addRule(Rule *rule)
{
    // TODO: check if already got such a rule
    rules_.push_back(Rule::ptr(rule));
}

bool StateMachine::addState(State *state)
{
    if (hasStateName(state->getName()))
    {
        std::cout << "StateMachine::" << __FUNCTION__ << ": Already has a state named " << state->getName() << std::endl;
        return false;
    }
    else
        states_[state->getName()] = State::ptr(state);
    if (states_.size() == 1)
        setCurrentStateName(state->getName());
    //else
    //    std::cout << "Have " << states_.size() << " states." << std::endl;
    return true;
}

const std::string &StateMachine::getCurrentStateName()
{
    return current_state_;
}

bool StateMachine::setCurrentStateName(const std::string &name)
{
    if (! hasStateName(name))
    {
        std::cout << "StateMachine::" << __FUNCTION__ << ": No such state " << name << std::endl;
        return false;
    }
    if (current_state_ != "")
    {
        getState(getCurrentStateName())->on_leaved_signal_();
    }
    current_state_ = name;
    getState(name)->on_entered_signal_();
    return true;
}

State *StateMachine::getState(const std::string &name)
{
    if (states_.find(name) == states_.end())
        return 0;
    else
        return states_[name].get();
}

bool StateMachine::hasStateName(const std::string &name)
{
    if (getState(name) == 0)
        return false;
    else
        return true;
}

bool StateMachine::triggerEvent(const std::string &event)
{
    bool ret = false;
    std::string current = getCurrentStateName();
    std::vector<Rule::ptr>::iterator iter;
    for (iter = rules_.begin(); iter < rules_.end(); ++iter)
    {
        Rule *rule = (*iter).get();
        if (rule->getEventName() == event)
        {
            if (rule->getStateFrom() == getCurrentStateName())
            {
                std::string stateTo = rule->getStateTo();
                if (hasStateName(stateTo))
                {
                    setCurrentStateName(stateTo);
                    on_state_changed_signal_(stateTo);
                    ret = true;
                    break;
                }
            }
        }
    }
    return ret;
}

} // end of namespace

} // end of namespace

