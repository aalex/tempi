/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 * Copyright (C) 2012 Emmanuel Durand
 *
 * This file is part of Tempi-plugins-base.
 *
 * This program is free software; you can redistither version 3 of the License, or
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
 * The SpatoscNode class.
 */

#ifndef __TEMPI_BASE_SPATOSC_H__
#define __TEMPI_BASE_SPATOSC_H__

#include "tempi/config.h" // include before check for HAVE_SPATOSC

#ifdef HAVE_SPATOSC

#include <iostream>
#include "tempi/utils.h"
#include "tempi/node.h"
#include "spatosc/wrapper.h"
#include "tempi/log.h"
#include <tr1/memory>

namespace tempi {
namespace plugins_base {

/**
 * The SpatoscNode controls a spatosc::Wrapper.
 */
class SpatoscNode : public Node
{
    public:
        SpatoscNode();
    private:
        virtual void onInit();
        std::tr1::shared_ptr<spatosc::Wrapper> wrapper_;
        virtual void processMessage(const char *inlet, const Message &message);
        virtual void doTick();
};

SpatoscNode::SpatoscNode() :
    Node()
{
    this->setShortDocumentation("Controls a SpatOSC scene.");
    this->setLongDocumentation(""
    "Supported messages:\n"
    "\n"
    "* clearScene\n"
    "* addTranslatorWithAddress, sss\n"
    "* addTranslatorWithoutAddress, ss\n"
    "* connect ,ss\n"
    "* createListener ,s\n"
    "* createSource ,s\n"
    "* debugPrint\n"
    "* deleteNode ,s\n"
    "* disconnect ,ss\n"
    "* flushMessages\n"
    "* removeNodeFloatProperty ,ss\n"
    "* removeNodeIntProperty ,ss\n"
    "* removeNodeStringProperty ,ss\n"
    "* removeTranslator ,s\n"
    "* setAutoConnect ,b\n"
    "* setConnectFilter ,s\n"
    "* setDopplerFactor ,ssf\n"
    "* setNodeActive ,sb\n"
    "* setNodeFloatProperty ,ssf\n"
    "* setNodeIntProperty ,ssi\n"
    "* setNodeOrientation ,sfff\n"
    "* setNodeStringProperty ,sss\n"
    "* setPosition ,sfff\n"
    "* setPositionAED ,sfff\n"
    "* setRadius ,sf\n"
    "* setScale ,fff\n"
    "* setSynchronous ,b\n"
    "* setTranslation ,fff\n"
    "* setTranslatorVerbose ,sb\n"
    "* setVerbose ,b\n"
    "");
    // TODO messageMatches(message, "setDefaultDistanceFactor", ""))
    // TODO messageMatches(message, "setDefaultDopplerFactor", ""))
    // TODO messageMatches(message, "setDefaultRolloffFactor", ""))
    // TODO messageMatches(message, "setDistanceFactor", ""))

    this->addInlet("0", "Calls to a instance of spatosc::Wrapper.");
    this->addOutlet("success", "Success or not of the last message. Outputs a boolean.");
}

void SpatoscNode::onInit()
{
    wrapper_.reset(new spatosc::Wrapper);
    wrapper_->setSynchronous(false);
}

void SpatoscNode::doTick()
{
    wrapper_->flushMessages();
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
    {
        std::ostringstream os;
        os << "SpatoscNode::" << __FUNCTION__ << ": " << message;
        Logger::log(DEBUG, os.str().c_str());
    }
    bool success = false;
    if (messageMatches(message, "clearScene", ""))
        wrapper_->clearScene();
    else if (messageMatches(message, "addTranslatorWithAddress", "sss"))
        success = wrapper_->addTranslatorWithAddress(message.getString(1), message.getString(2), message.getString(3));
    else if (messageMatches(message, "addTranslatorWithoutAddress", "ss"))
        success = wrapper_->addTranslatorWithoutAddress(message.getString(1), message.getString(2));
    else if (messageMatches(message, "connect", "ss"))
        success = wrapper_->connect(message.getString(1), message.getString(2));
    else if (messageMatches(message, "createListener", "s"))
        success = wrapper_->createListener(message.getString(1));
    else if (messageMatches(message, "createSource", "s"))
        success = wrapper_->createSource(message.getString(1));
    else if (messageMatches(message, "debugPrint", ""))
    {
        wrapper_->debugPrint();
        success = true;
    }
    else if (messageMatches(message, "deleteNode", "s"))
        success = wrapper_->deleteNode(message.getString(1));
    else if (messageMatches(message, "disconnect", "ss"))
        success = wrapper_->disconnect(message.getString(1), message.getString(2));
    else if (messageMatches(message, "flushMessages", ""))
        success = wrapper_->flushMessages();
    else if (messageMatches(message, "removeNodeFloatProperty", "ss"))
        success = wrapper_->removeNodeFloatProperty(message.getString(1), message.getString(2));
    else if (messageMatches(message, "removeNodeIntProperty", "ss"))
        success = wrapper_->removeNodeIntProperty(message.getString(1), message.getString(2));
    else if (messageMatches(message, "removeNodeStringProperty", "ss"))
        success = wrapper_->removeNodeStringProperty(message.getString(1), message.getString(2));
    else if (messageMatches(message, "removeTranslator", "s"))
        success = wrapper_->removeTranslator(message.getString(1));
    else if (messageMatches(message, "setAutoConnect", "b"))
        success = wrapper_->setAutoConnect(message.getBoolean(1));
    else if (messageMatches(message, "setConnectFilter", "s"))
        success = wrapper_->setConnectFilter(message.getString(1));
    // TODO else if (messageMatches(message, "setDefaultDistanceFactor", ""))
    // TODO else if (messageMatches(message, "setDefaultDopplerFactor", ""))
    // TODO else if (messageMatches(message, "setDefaultRolloffFactor", ""))
    // TODO else if (messageMatches(message, "setDistanceFactor", ""))
    // TODO     success = wrapper_->setDistanceFactor(message.getString(1), (float) message.getFloat(2));
    else if (messageMatches(message, "setDopplerFactor", "ssf"))
        success = wrapper_->setDopplerFactor(message.getString(1), message.getString(2), (float) message.getFloat(3));
    else if (messageMatches(message, "setNodeActive", "sb"))
        success = wrapper_->setNodeActive(message.getString(1), message.getBoolean(2));
    else if (messageMatches(message, "setNodeFloatProperty", "ssf"))
        success = wrapper_->setNodeFloatProperty(message.getString(1), message.getString(2), (float) message.getFloat(3));
    else if (messageMatches(message, "setNodeIntProperty", "ssi"))
        success = wrapper_->setNodeIntProperty(message.getString(1), message.getString(2), message.getInt(3));
    else if (messageMatches(message, "setNodeOrientation", "sfff"))
        success = wrapper_->setNodeOrientation(message.getString(1), (float) message.getFloat(2), (float) message.getFloat(3), (float) message.getFloat(4));
    else if (messageMatches(message, "setNodeStringProperty", "sss"))
        success = wrapper_->setNodeStringProperty(message.getString(1), message.getString(2), message.getString(3));
    else if (messageMatches(message, "setPosition", "sfff"))
        success = wrapper_->setPosition(message.getString(1), (float) message.getFloat(2), (float) message.getFloat(3), (float) message.getFloat(4));
    else if (messageMatches(message, "setPositionAED", "sfff"))
        success = wrapper_->setPositionAED(message.getString(1), (float) message.getFloat(2), (float) message.getFloat(3), (float) message.getFloat(4));
    else if (messageMatches(message, "setRadius", "sf"))
        success = wrapper_->setRadius(message.getString(1), (float) message.getFloat(2));
    else if (messageMatches(message, "setScale", "fff"))
    {
        wrapper_->setScale((float) message.getFloat(1), (float) message.getFloat(2), (float) message.getFloat(3));
        success = true;
    }
    else if (messageMatches(message, "setSynchronous", "b"))
    {
        wrapper_->setSynchronous(message.getBoolean(1));
        success = true;
    }
    else if (messageMatches(message, "setTranslation", "fff"))
    {
        wrapper_->setTranslation((float) message.getFloat(1), (float) message.getFloat(2), (float) message.getFloat(3));
        success = true;
    }
    else if (messageMatches(message, "setTranslatorVerbose", "sb"))
    {
        wrapper_->setTranslatorVerbose(message.getString(1), message.getBoolean(2));
        success = true;
    }
    else if (messageMatches(message, "setVerbose", "b"))
        success = wrapper_->setVerbose(message.getBoolean(1));
    if (! success)
        std::cerr << "[spatosc]: could not handle message " << message << std::endl;
    {
        std::ostringstream os;
        os << "SpatoscNode::" << __FUNCTION__ << ": " << "Success: " << success;
        Logger::log(DEBUG, os.str().c_str());
    }
    this->output("success", Message("b", success));
}

} // end of namespace
} // end of namespace

#endif // HAVE_SPATOSC
#endif // ifndef

