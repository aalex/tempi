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

#include <string>
#include "tempi/fsm/rule.h"

namespace tempi
{

namespace fsm
{

Rule::Rule(const std::string &eventName, const std::string &stateFrom, const std::string &stateTo) :
    eventName_(eventName),
    stateTo_(stateTo),
    stateFrom_(stateFrom)
{
    // pass
}

const std::string &Rule::getEventName()
{
    return eventName_;
}

void Rule::setEventName(const std::string &name)
{
    eventName_ = name;
}

const std::string &Rule::getStateFrom()
{
    return stateFrom_;
}

void Rule::setStateFrom(const std::string &state)
{
    stateFrom_= state;
}

const std::string &Rule::getStateTo()
{
    return stateTo_;
}
void Rule::setStateTo(const std::string &state)
{
    stateTo_= state;
}

} // end of namespace

} // end of namespace

