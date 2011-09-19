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

#include "tempi/tempi.h"
#include <boost/python.hpp>

BOOST_PYTHON_MODULE(tempi)
{
    using namespace boost::python;
    using namespace tempi;
    def("hello", hello);
    def("get_version", get_version);

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
}

