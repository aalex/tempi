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

Wrapper::Wrapper(bool synchronous) :
    synchronous_(synchronous)
{
    if (synchronous_)
        scheduler_.reset(new SynchronousScheduler);
    else
    {
        scheduler_.reset(new ThreadedScheduler);
        ThreadedScheduler *threaded = dynamic_cast<ThreadedScheduler*>(scheduler_.get());
        threaded->start(5); // 5 ms
    }
}

Wrapper::~Wrapper()
{
    if (! synchronous_)
    {
        ThreadedScheduler *threaded = dynamic_cast<ThreadedScheduler*>(scheduler_.get());
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ <<
            "(): Waiting for Scheduler's thread to join.";
        Logger::log(DEBUG, os);
        threaded->stop();
    }
}

bool Wrapper::setLogLevel(const char *level)
{
    LogLevel new_level;
    if (utils::stringsMatch(level, "DEBUG"))
        new_level = DEBUG;
    else if (utils::stringsMatch(level, "INFO"))
        new_level = INFO;
    else if (utils::stringsMatch(level, "NOTICE"))
        new_level = NOTICE;
    else if (utils::stringsMatch(level, "WARNING"))
        new_level = WARNING;
    else if (utils::stringsMatch(level, "CRITICAL"))
        new_level = CRITICAL;
    else if (utils::stringsMatch(level, "ERROR"))
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

bool Wrapper::loadGraph(const char *name, const char *fileName)
{
    tempi::ScopedLock::ptr lock = scheduler_->acquireLock();
    if (scheduler_->hasGraph(name))
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ <<
            ": already has a graph \"" << name << "\"";
        tempi::Logger::log(ERROR, os);
        return false;
    }
    // Check for XML file
    if (! saver_->fileExists(fileName))
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
    scheduler_->createGraph(name);
    Graph::ptr graph = scheduler_->getGraph(name);

    // load graph
    saver_->load(*(graph.get()), fileName);
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
        SynchronousScheduler *scheduler = dynamic_cast<SynchronousScheduler*>(scheduler_.get());
        scheduler->tick();
    }
    else
    {
        std::ostringstream os;
        os << "Wrapper" << __FUNCTION__ << ": " <<
            "Do not call tick if using a synchronous is false. ";
        tempi::Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::listGraphs(std::vector<std::string> &names) const
{
    names = this->scheduler_->listGraphs();
    return true;
}

bool Wrapper::createGraph(const char *name)
{
    try
    {
        this->scheduler_->createGraph(name);
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

bool Wrapper::destroyGraph(const char *name)
{
    try
    {
        this->scheduler_->removeGraph(name);
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

bool Wrapper::hasGraph(const char *name)
{
    return this->scheduler_->hasGraph(name);
}

bool Wrapper::createNode(const char *graph, const char *nodeType,
    const char *nodeName)
{
    try
    {
        return this->scheduler_->getGraph(graph)->addNode(nodeType, nodeName);
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::destroyNode(const char *graph, const char *nodeName)
{
    try
    {
        return this->scheduler_->getGraph(graph)->
            deleteNode(nodeName);
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::messageInlet(const char *graph, const char *nodeName,
    const char *nodeInlet, const Message &message)
{
    try
    {
        return this->scheduler_->getGraph(graph)->
            message(nodeName, nodeInlet, message);
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::connect(const char *graph,
    const char *from, const char *outlet,
    const char *to, const char *inlet)
{
    try
    {
        return this->scheduler_->getGraph(graph)->
            connect(from, outlet, to, inlet);
    }
    catch (const BaseException &e)
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
}

bool Wrapper::disconnect(const char *graph,
    const char *from, const char *outlet,
    const char *to, const char *inlet)
{
    try
    {
        return this->scheduler_->getGraph(graph)->
            disconnect(from, outlet, to, inlet);
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
    const char *graph,
    const char *node,
    const char *attribute,
    const Message &value)
{
    try
    {
        return this->scheduler_->getGraph(graph)->
            setNodeAttribute(node, attribute, value);
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
    const char *graph,
    const char *node,
    std::vector<std::string> &names) const
{
    try
    {
        names = this->scheduler_->getGraph(graph)->
            getNode(node)->listAttributes();
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
    const char *graph,
    const char *node,
    std::vector<std::string> &names) const
{
    try
    {
        names = this->scheduler_->getGraph(graph)->
            getNode(node)->listMethods();
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
    const char *graph,
    const char *node,
    std::vector<std::string> &names) const
{
    try
    {
        names = this->scheduler_->getGraph(graph)->
            getNode(node)->listInlets();
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
    const char *graph,
    const char *node,
    std::vector<std::string> &names) const
{
    try
    {
        names = this->scheduler_->getGraph(graph)->
            getNode(node)->listOutlets();
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
    const char *graph,
    const char *node,
    std::string &typeName) const
{
    try
    {
        typeName = this->scheduler_->getGraph(graph)->
            getNode(node)->getTypeName();
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
    const char *graph,
    const char *node,
    const char *attribute,
    Message &value) const
{
    try
    {
        value = this->scheduler_->getGraph(graph)->
            getNode(node)->getAttributeValue(attribute);
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
    const char *graph,
    const char *node,
    const char *attribute,
    std::string &value) const
{
    try
    {
        value = this->scheduler_->getGraph(graph)->
            getNode(node)->getAttribute(attribute)->
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
    const char *graph,
    const char *node,
    const char *inlet,
    std::string &value) const
{
    try
    {
        value = this->scheduler_->getGraph(graph)->
            getNode(node)->getInlet(inlet)->
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
    const char *graph,
    const char *node,
    const char *outlet,
    std::string &value) const
{
    try
    {
        value = this->scheduler_->getGraph(graph)->
            getNode(node)->getOutlet(outlet)->
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


bool Wrapper::addLibraryPath(const char *path)
{
    std::ostringstream os;
    os << "Wrapper." << __FUNCTION__ << ": TODO";
    Logger::log(ERROR, os);
    return false;
}

bool Wrapper::loadLibrary(const char *name)
{
    std::ostringstream os;
    os << "Wrapper." << __FUNCTION__ << ": TODO";
    Logger::log(ERROR, os);
    return false;
}

bool Wrapper::saveGraph(const char *name, const char *fileName)
{
    tempi::ScopedLock::ptr lock = scheduler_->acquireLock();
    if (! scheduler_->hasGraph(name))
    {
        std::ostringstream os;
        os << "Wrapper." << __FUNCTION__ <<
            ": no such graph \"" << name << "\"";
        tempi::Logger::log(ERROR, os);
        return false;
    }
    Graph::ptr graph = scheduler_->getGraph(name);

    // save graph
    return saver_->save(*(graph.get()), fileName);
}

} // end of namespace

