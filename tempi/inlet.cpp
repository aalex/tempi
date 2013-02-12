/*
 * Copyright (C) 2011 Alexandre Quessy
 *
 * This file is part of Tempi.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software ither version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tempi.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "tempi/inlet.h"
#include "tempi/outlet.h"
#include "tempi/log.h"
#include <iostream>

namespace tempi {

Inlet::Inlet(const char *name, const char *short_documentation,
    const char * long_documentation, const char *types_filter) :
        Pad(name, short_documentation, long_documentation)
{
    this->setTypesFilter(types_filter);
}

bool Inlet::setTypesFilter(const char *types_filter)
{
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "Inlet." << __FUNCTION__ << "(" << types_filter << ") for inlet " << this->getName();
        Logger::log(DEBUG, os);
    }
    return this->filter_.setFilter(types_filter);
}

std::string Inlet::getTypesFilter() const
{
    return this->filter_.getFilter();
}

Inlet::~Inlet()
{
    // just in case.
    this->disconnectAll();
}

Pad::TriggeredSignal &Inlet::getOnTriggeredSignal()
{
    return on_triggered_signal_;
}

void Inlet::onMessageReceivedFromSource(Pad *outlet, const Message &message)
{
    std::string outlet_name;
    if (outlet == 0)
    {
        outlet_name = "NULL";
    }
    else
    {
        outlet_name = outlet->getName();
    }
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "Inlet." << __FUNCTION__ << "(" << outlet_name << ", " << message << ")";
        Logger::log(DEBUG, os);
    }
    bool type_ok = false;
    if (this->filter_.isWildcard())
        type_ok = true;
    else if (this->filter_.matchesFilter(message))
        type_ok = true;
    if (type_ok)
    {
        if (Logger::isEnabledFor(DEBUG))
        {
            std::ostringstream os;
            os << "Inlet." << __FUNCTION__ << ": type is ok.";
            Logger::log(DEBUG, os);
        }
        // FIXME: this is confusin
        this->triggerInlet(message);
    }
    else
    {
        Message casted;
        if (this->filter_.tryCast(message, casted))
        {
            if (Logger::isEnabledFor(INFO))
            {
                std::ostringstream os;
                os << "Inlet." << __FUNCTION__ << ": did cast message to desired type:" << casted;
                Logger::log(INFO, os);
            }
            // FIXME: this is confusing
            this->triggerInlet(casted);
        }
        else
        {
            std::ostringstream os;
            os << "Inlet." << __FUNCTION__ << ": Could not handle message of wrong type " << message.getTypes() << ": " << message;
            Logger::log(ERROR, os);
            // TODO: return false
        }
    }
    // TODO: return true;
}

void Inlet::triggerInlet(const Message &message)
{
    on_triggered_signal_(dynamic_cast<Pad *>(this), message);
}

bool Inlet::connect(Outlet::ptr source)
{
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "Inlet." << __FUNCTION__ << "(" << source->getName() << ")";
        Logger::log(DEBUG, os);
    }
    if (! this->isConnected(source))
    {
        this->sources_.push_back(source);
        source.get()->getOnPadTriggeredSignal().connect(boost::bind(&Inlet::onMessageReceivedFromSource, this, _1, _2));
        return true;
    }
    return false;
}

void Inlet::disconnectAll()
{
    on_triggered_signal_.disconnect_all_slots();
    sources_.clear();
}

bool Inlet::disconnect(Outlet::ptr source)
{
    if (isConnected(source))
    {
        source.get()->getOnPadTriggeredSignal().disconnect(boost::bind(&Inlet::onMessageReceivedFromSource, this, _1, _2));
        sources_.erase(std::find(sources_.begin(), sources_.end(), source));
        return true;
    }
    else
        return false;
}

bool Inlet::isConnected(Outlet::ptr source)
{
    return std::find(sources_.begin(), sources_.end(), source) != sources_.end();
}

} // end of namespace

