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

#include "tempi/graph.h"
#include "tempi/utils.h"
#include "tempi/wrapper.h"
#include "tempi/config.h"
#include "tempi/synchronousscheduler.h"
#include "tempi/threadedscheduler.h"
#include "tempi/config.h"
#include "tempi/log.h"
#include <sstream>
#include <iostream>

namespace tempi {

// TODO: allow to set synchronous
Wrapper::Wrapper()
{
    synchronous_ = false;
    // FIXME: setSynchronous must only be called once for now!!
    this->setSynchronous(synchronous_);
}

bool Wrapper::setSynchronous(bool synchronous)
{
    synchronous_ = synchronous;
    // FIXME
    // if (scheduler_)
    //    delete scheduler_;
    if (synchronous_)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " <<
            "Using SynchronousScheduler.";
        Logger::log(INFO, os);
        scheduler_ = new SynchronousScheduler;
    }
    else
    {
        scheduler_ = new ThreadedScheduler;
        ThreadedScheduler *threaded = dynamic_cast<ThreadedScheduler*>(scheduler_);
        threaded->start(5); // 5 ms
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " <<
            "Using ThreadedScheduler. ";
            "Starting ThreadedScheduler";
        Logger::log(INFO, os);
    }
}

Wrapper::~Wrapper()
{
    if (! synchronous_)
    {
        ThreadedScheduler *threaded = dynamic_cast<ThreadedScheduler*>(scheduler_);
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ <<
            "(): Waiting for Scheduler's thread to join.";
        Logger::log(DEBUG, os);
        threaded->stop();
    }
    delete scheduler_;
}

bool Wrapper::setLogLevel(const std::string &level)
{
    LogLevel new_level;
    if (utils::stringsMatch(level.c_str(), "DEBUG"))
        new_level = DEBUG;
    else if (utils::stringsMatch(level.c_str(), "INFO"))
        new_level = INFO;
    else if (utils::stringsMatch(level.c_str(), "NOTICE"))
        new_level = NOTICE;
    else if (utils::stringsMatch(level.c_str(), "WARNING"))
        new_level = WARNING;
    else if (utils::stringsMatch(level.c_str(), "CRITICAL"))
        new_level = CRITICAL;
    else if (utils::stringsMatch(level.c_str(), "ERROR"))
        new_level = ERROR;
    else
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ <<
            "Invalid log level: " << level;
        Logger::log(ERROR, os);
        return false;
    }
    {
        tempi::Logger::getInstance().setLevel(new_level);
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ <<
            "Set log level to " << level;
        Logger::log(INFO, os);
    }
    return true;
}

bool Wrapper::loadGraph(const std::string &name, const std::string &fileName)
{
    tempi::ScopedLock::ptr lock = scheduler_->acquireLock();
    if (scheduler_->hasGraph(name.c_str()))
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ <<
            ": already has a graph \"" << name << "\"";
        tempi::Logger::log(ERROR, os);
        return false;
    }
    // Check for XML file
    if (! saver_->fileExists(fileName.c_str()))
    {
        std::ostringstream os;
        os << "Wrapper" << __FUNCTION__ << ": " <<
            "File \"" << fileName << "\" not found!";
        tempi::Logger::log(ERROR, os);
        return false;
    }
    {
        std::ostringstream os;
        os << "Wrapper" << __FUNCTION__ << ": " <<
            "Found " << fileName;
        tempi::Logger::log(DEBUG, os);
    }
    scheduler_->createGraph(name.c_str());
    Graph::ptr graph = scheduler_->getGraph(name.c_str());

    // load graph
    saver_->load(*(graph.get()), fileName.c_str());
    graph->tick(); // FIXME
    {
        std::ostringstream os;
        os << "Wrapper" << __FUNCTION__ << ": " <<
            "Loaded " << fileName << " into graph " << name;
        tempi::Logger::log(INFO, os);
    }
    return true;
}

bool Wrapper::getSynchronous() const
{
    return synchronous_;
}

bool Wrapper::tick()
{
    if (synchronous_)
    {
        SynchronousScheduler *scheduler = dynamic_cast<SynchronousScheduler*>(scheduler_);
        scheduler->tick();
    }
    else
    {
        std::ostringstream os;
        os << "Wrapper" << __FUNCTION__ << ": " <<
            "Do not call tick if using a synchronous is false.";
        tempi::Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::listGraphs(std::vector<std::string> &names) const
{
    names = this->scheduler_->listGraphs();
    return true;
}

bool Wrapper::createGraph(const std::string &name)
{
    try
    {
        this->scheduler_->createGraph(name.c_str());
        return true;
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::destroyGraph(const std::string &name)
{
    try
    {
        this->scheduler_->removeGraph(name.c_str());
        return true;
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::hasGraph(const std::string &name)
{
    return this->scheduler_->hasGraph(name.c_str());
}

bool Wrapper::createNode(const std::string &graph, const std::string &nodeType,
    const std::string &nodeName)
{
    try
    {
        return this->scheduler_->getGraph(graph.c_str())->addNode(nodeType.c_str(), nodeName.c_str());
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::destroyNode(const std::string &graph, const std::string &nodeName)
{
    try
    {
        return this->scheduler_->getGraph(graph.c_str())->
            deleteNode(nodeName.c_str());
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::messageInlet(const std::string &graph, const std::string &nodeName,
    const std::string &nodeInlet, const Message &message)
{
    try
    {
        return this->scheduler_->getGraph(graph.c_str())->
            message(nodeName.c_str(), nodeInlet.c_str(), message);
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::connect(const std::string &graph,
    const std::string &from, const std::string &outlet,
    const std::string &to, const std::string &inlet)
{
    try
    {
        return this->scheduler_->getGraph(graph.c_str())->
            connect(from.c_str(), outlet.c_str(), to.c_str(), inlet.c_str());
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::disconnect(const std::string &graph,
    const std::string &from, const std::string &outlet,
    const std::string &to, const std::string &inlet)
{
    try
    {
        return this->scheduler_->getGraph(graph.c_str())->
            disconnect(from.c_str(), outlet.c_str(), to.c_str(), inlet.c_str());
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::listNodeTypes(std::vector<std::string> &names) const
{
    std::map<std::string, AbstractNodeType::ptr> entries = 
        this->scheduler_->getFactory()->getEntries();
    names.clear();
    std::map<std::string, AbstractNodeType::ptr>::const_iterator iter;
    for (iter = entries.begin(); iter != entries.end(); ++iter)
        names.push_back((*iter).first);
    return true;
}

bool Wrapper::setNodeAttributeValue(
    const std::string &graph,
    const std::string &node,
    const std::string &attribute,
    const Message &value)
{
    try
    {
        return this->scheduler_->getGraph(graph.c_str())->
            setNodeAttribute(node.c_str(), attribute.c_str(), value);
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::listNodeAttributes(
    const std::string &graph,
    const std::string &node,
    std::vector<std::string> &names) const
{
    try
    {
        names = this->scheduler_->getGraph(graph.c_str())->
            getNode(node.c_str())->listAttributes();
        return true;
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::listNodeMethods(
    const std::string &graph,
    const std::string &node,
    std::vector<std::string> &names) const
{
    try
    {
        names = this->scheduler_->getGraph(graph.c_str())->
            getNode(node.c_str())->listMethods();
        return true;
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::listNodeInlets(
    const std::string &graph,
    const std::string &node,
    std::vector<std::string> &names) const
{
    try
    {
        names = this->scheduler_->getGraph(graph.c_str())->
            getNode(node.c_str())->listInlets();
        return true;
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::listNodeOutlets(
    const std::string &graph,
    const std::string &node,
    std::vector<std::string> &names) const
{
    try
    {
        names = this->scheduler_->getGraph(graph.c_str())->
            getNode(node.c_str())->listOutlets();
        return true;
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::getNodeTypeName(
    const std::string &graph,
    const std::string &node,
    std::string &typeName) const
{
    try
    {
        typeName = this->scheduler_->getGraph(graph.c_str())->
            getNode(node.c_str())->getTypeName();
        return true;
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::getNodeAttributeValue(
    const std::string &graph,
    const std::string &node,
    const std::string &attribute,
    Message &value) const
{
    try
    {
        value = this->scheduler_->getGraph(graph.c_str())->
            getNode(node.c_str())->getAttributeValue(attribute.c_str());
        return true;
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::getNodeAttributeDocumentation(
    const std::string &graph,
    const std::string &node,
    const std::string &attribute,
    std::string &value) const
{
    try
    {
        value = this->scheduler_->getGraph(graph.c_str())->
            getNode(node.c_str())->getAttribute(attribute.c_str())->
            getShortDocumentation();
        return true;
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::getNodeInletDocumentation(
    const std::string &graph,
    const std::string &node,
    const std::string &inlet,
    std::string &value) const
{
    try
    {
        value = this->scheduler_->getGraph(graph.c_str())->
            getNode(node.c_str())->getInlet(inlet.c_str())->
            getDocumentation();
        return true;
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::getNodeOutletDocumentation(
    const std::string &graph,
    const std::string &node,
    const std::string &outlet,
    std::string &value) const
{
    try
    {
        value = this->scheduler_->getGraph(graph.c_str())->
            getNode(node.c_str())->getOutlet(outlet.c_str())->
            getDocumentation();
        return true;
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}


bool Wrapper::addLibraryPath(const std::string &path)
{
    std::ostringstream os;
    os << "Wrapper." << __FUNCTION__ << ": TODO";
    Logger::log(ERROR, os);
    return false;
}

bool Wrapper::loadLibrary(const std::string &name)
{
    std::ostringstream os;
    os << "Wrapper." << __FUNCTION__ << ": TODO";
    Logger::log(ERROR, os);
    return false;
}

bool Wrapper::saveGraph(const std::string &name, const std::string &fileName)
{
    tempi::ScopedLock::ptr lock = scheduler_->acquireLock();
    if (! scheduler_->hasGraph(name.c_str()))
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ <<
            ": no such graph \"" << name << "\"";
        tempi::Logger::log(ERROR, os);
        return false;
    }
    Graph::ptr graph = scheduler_->getGraph(name.c_str());

    // save graph
    return saver_->save(*(graph.get()), fileName.c_str());
}

} // end of namespace

