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

#include "tempi/message.h"
#include "tempi/wrapper.h"
#include "tempi/tempi.h"
#include <boost/python.hpp>

BOOST_PYTHON_MODULE(tempi)
{
    using namespace boost::python;
    using namespace tempi;
    // def("hello", hello);
    def("get_version", get_version);

    enum_<AtomType>("AtomType")
        .value("BOOLEAN", BOOLEAN)
        .value("CHAR", CHAR)
        .value("DOUBLE", DOUBLE)
        .value("FLOAT", FLOAT)
        .value("INT", INT)
        .value("LONG", LONG)
        .value("STRING", STRING)
        ;

    class_<Message>("Message")
        .def("appendBoolean", &Message::appendBoolean)
        .def("appendChar", &Message::appendChar)
        .def("appendDouble", &Message::appendDouble)
        .def("appendFloat", &Message::appendFloat)
        .def("appendInt", &Message::appendInt)
        .def("appendLong", &Message::appendLong)
        .def("appendString", &Message::appendString)
        .def("getBoolean", &Message::getBoolean)
        .def("getChar", &Message::getChar)
        .def("getDouble", &Message::getDouble)
        .def("getFloat", &Message::getFloat)
        .def("getInt", &Message::getInt)
        .def("getLong", &Message::getLong)
        .def("getString", &Message::getString)
        .def("getTypes", &Message::getTypes)
        .def("getSize", &Message::getSize)
        ;
        //TODO: .def("getAtomType", &Message::AtomType)

    class_<Wrapper>("Wrapper") // , init<bool>())
        .def("addLibraryPath", &Wrapper::addLibraryPath)
        .def("connect", &Wrapper::connect)
        .def("createGraph", &Wrapper::createGraph)
        .def("createNode", &Wrapper::createNode)
        .def("destroyGraph", &Wrapper::destroyGraph)
        .def("destroyNode", &Wrapper::destroyNode)
        .def("disconnect", &Wrapper::disconnect)
        .def("getNodeAttributeDocumentation", &Wrapper::getNodeAttributeDocumentation)
        .def("getNodeAttributeValue", &Wrapper::getNodeAttributeValue)
        .def("getNodeInletDocumentation", &Wrapper::getNodeInletDocumentation)
        .def("getNodeOutletDocumentation", &Wrapper::getNodeOutletDocumentation)
        .def("getNodeTypeName", &Wrapper::getNodeTypeName)
        .def("hasGraph", &Wrapper::hasGraph)
        .def("listGraphs", &Wrapper::listGraphs)
        .def("listNodeAttributes", &Wrapper::listNodeAttributes)
        .def("listNodeInlets", &Wrapper::listNodeInlets)
        .def("listNodeMethods", &Wrapper::listNodeMethods)
        .def("listNodeOutlets", &Wrapper::listNodeOutlets)
        .def("listNodeTypes", &Wrapper::listNodeTypes)
        .def("loadGraph", &Wrapper::loadGraph)
        .def("loadLibrary", &Wrapper::loadLibrary)
        .def("messageInlet", &Wrapper::messageInlet)
        .def("saveGraph", &Wrapper::saveGraph)
        .def("setLogLevel", &Wrapper::setLogLevel)
        .def("setSynchronous", &Wrapper::setSynchronous)
        .def("setNodeAttributeValue", &Wrapper::setNodeAttributeValue)
        .def("tick", &Wrapper::tick)
        ;
}

