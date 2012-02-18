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

#ifdef HAVE_SPATOSC

#include <iostream>
#include "tempi/base/spatosc/spatoscnode.h"
#include "tempi/utils.h"
#include "spatosc/wrapper.h"

namespace tempi {
namespace base {

struct SpatoscNode_internals
{
    public:
        spatosc::Wrapper wrapper_;
};

SpatoscNode::SpatoscNode() :
    Node()
{
    internals_ = new SpatoscNode_internals;
    this->setShortDocumentation("Controls a SpatOSC scene.");
    this->addInlet("0", "Calls to a instance of spatosc::Wrapper.");
    this->addOutlet("success", "Success or not of the last message. Outputs a boolean.");
}

SpatoscNode::~SpatoscNode()
{
    delete internals_;
}

static bool messageMatches(const Message &message, const char *selector, const char *argsTypeTag)
{
    std::string typetag("s");
    typetag += argsTypeTag;
    if (! message.typesMatch(typetag.c_str()))
    {
        //std::cerr << "Message do not match expected type tag" << std::endl;
        return false;
    }
    if (! utils::stringsMatch(message.getString(0).c_str(), selector))
    {

        //std::cerr << "Message do not match expected selector." << std::endl;
        return false;
    }
    return true;
}

void SpatoscNode::processMessage(const char *inlet, const Message &message)
{
    bool success = false;
    if (messageMatches(message, "clearScene", ""))
        internals_->wrapper_.clearScene();
    else if (messageMatches(message, "addTranslatorWithAddress", "sss"))
        success = internals_->wrapper_.addTranslatorWithAddress(message.getString(1), message.getString(2), message.getString(3));
    else if (messageMatches(message, "addTranslatorWithoutAddress", "ss"))
        success = internals_->wrapper_.addTranslatorWithoutAddress(message.getString(1), message.getString(2));
    else if (messageMatches(message, "connect", "ss"))
        success = internals_->wrapper_.connect(message.getString(1), message.getString(2));
    else if (messageMatches(message, "createListener", "s"))
        success = internals_->wrapper_.createListener(message.getString(1));
    else if (messageMatches(message, "createSource", "s"))
        success = internals_->wrapper_.createSource(message.getString(1));
    else if (messageMatches(message, "debugPrint", ""))
    {
        internals_->wrapper_.debugPrint();
        success = true;
    }
    else if (messageMatches(message, "deleteNode", "s"))
        success = internals_->wrapper_.deleteNode(message.getString(1));
    else if (messageMatches(message, "disconnect", "ss"))
        success = internals_->wrapper_.disconnect(message.getString(1), message.getString(2));
    else if (messageMatches(message, "flushMessages", ""))
        success = internals_->wrapper_.flushMessages();
    else if (messageMatches(message, "removeNodeFloatProperty", "ss"))
        success = internals_->wrapper_.removeNodeFloatProperty(message.getString(1), message.getString(2));
    else if (messageMatches(message, "removeNodeIntProperty", "ss"))
        success = internals_->wrapper_.removeNodeIntProperty(message.getString(1), message.getString(2));
    else if (messageMatches(message, "removeNodeStringProperty", "ss"))
        success = internals_->wrapper_.removeNodeStringProperty(message.getString(1), message.getString(2));
    else if (messageMatches(message, "removeTranslator", "s"))
        success = internals_->wrapper_.removeTranslator(message.getString(1));
    else if (messageMatches(message, "setAutoConnect", "b"))
        success = internals_->wrapper_.setAutoConnect(message.getBoolean(1));
    else if (messageMatches(message, "setConnectFilter", "s"))
        success = internals_->wrapper_.setConnectFilter(message.getString(1));
    // TODO else if (messageMatches(message, "setDefaultDistanceFactor", ""))
    // TODO else if (messageMatches(message, "setDefaultDopplerFactor", ""))
    // TODO else if (messageMatches(message, "setDefaultRolloffFactor", ""))
    // TODO else if (messageMatches(message, "setDistanceFactor", ""))
    // TODO     success = internals_->wrapper_.setDistanceFactor(message.getString(1), message.getFloat(2));
    else if (messageMatches(message, "setDopplerFactor", "ssf"))
        success = internals_->wrapper_.setDopplerFactor(message.getString(1), message.getString(2), message.getFloat(3));
    else if (messageMatches(message, "setNodeActive", "sb"))
        success = internals_->wrapper_.setNodeActive(message.getString(1), message.getBoolean(2));
    else if (messageMatches(message, "setNodeFloatProperty", "ssf"))
        success = internals_->wrapper_.setNodeFloatProperty(message.getString(1), message.getString(2), message.getFloat(3));
    else if (messageMatches(message, "setNodeIntProperty", "ssi"))
        success = internals_->wrapper_.setNodeIntProperty(message.getString(1), message.getString(2), message.getInt(3));
    else if (messageMatches(message, "setNodeOrientation", "sfff"))
        success = internals_->wrapper_.setNodeOrientation(message.getString(1), message.getFloat(2), message.getFloat(3), message.getFloat(4));
    else if (messageMatches(message, "setNodeStringProperty", "sss"))
        success = internals_->wrapper_.setNodeStringProperty(message.getString(1), message.getString(2), message.getString(3));
    else if (messageMatches(message, "setPosition", "sfff"))
        success = internals_->wrapper_.setPosition(message.getString(1), message.getFloat(2), message.getFloat(3), message.getFloat(4));
    else if (messageMatches(message, "setPositionAED", "sfff"))
        success = internals_->wrapper_.setPositionAED(message.getString(1), message.getFloat(2), message.getFloat(3), message.getFloat(4));
    else if (messageMatches(message, "setRadius", "sf"))
        success = internals_->wrapper_.setRadius(message.getString(1), message.getFloat(2));
    else if (messageMatches(message, "setScale", "fff"))
    {
        internals_->wrapper_.setScale(message.getFloat(1), message.getFloat(2), message.getFloat(3));
        success = true;
    }
    else if (messageMatches(message, "setSynchronous", "b"))
    {
        internals_->wrapper_.setSynchronous(message.getBoolean(1));
        success = true;
    }
    else if (messageMatches(message, "setTranslation", "fff"))
    {
        internals_->wrapper_.setTranslation(message.getFloat(1), message.getFloat(2), message.getFloat(3));
        success = true;
    }
    else if (messageMatches(message, "setTranslatorVerbose", "sb"))
    {
        internals_->wrapper_.setTranslatorVerbose(message.getString(1), message.getBoolean(2));
        success = true;
    }
    else if (messageMatches(message, "setVerbose", "b"))
        success = internals_->wrapper_.setVerbose(message.getBoolean(1));
    if (! success)
        std::cerr << "[spatosc]: could not handle message " << message << std::endl;
    this->output("success", Message("b", success));
}

} // end of namespace
} // end of namespace

#endif // HAVE_SPATOSC

