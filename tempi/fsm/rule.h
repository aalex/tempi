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
 * The Rule class.
 */
#ifndef __TEMPI_RULE_H__
#define __TEMPI_RULE_H__

#include <string>
#include <tr1/memory>

namespace tempi
{

namespace fsm
{

/**
 * A Rule is used to configure how to switch between States in a StateMachine, upon Events occurrign.
 */
class Rule 
{
    public:
        typedef std::tr1::shared_ptr<Rule> ptr;
        Rule(const std::string &eventName, const std::string &stateFrom, const std::string &stateTo);
        const std::string &getEventName();
        void setEventName(const std::string &name);
        const std::string &getStateFrom();
        void setStateFrom(const std::string &state);
        const std::string &getStateTo();
        void setStateTo(const std::string &state);
    private:
        std::string eventName_;
        std::string stateTo_;
        std::string stateFrom_;
};

} // end of namespace

} // end of namespace

#endif // ifndef

