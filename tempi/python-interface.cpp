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

#include "tempi/tempi.h"
#include <boost/python.hpp>

BOOST_PYTHON_MODULE(tempi)
{
    using namespace boost::python;
    using namespace tempi;
    def("hello", hello);
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

    // class_<Track>("Track")
    //     .def("reset", &Track::reset)
    //     .def("add", &Track::add)
    //     .def("numberOfEvents", &Track::numberOfEvents)
    //     .def("getDuration", &Track::getDuration)
    //     ;

        //TODO: .def("getFirst", &Track::getFirst, return_internal_reference<return_internal_reference>)
        //TODO: .def("getLast", &Track::getLast)
        //TODO: .def("getClosest", &Track::getClosest)

    // class_<Recorder>("Recorder", init<Track*>())
    //     .def("setTrack", &Recorder::setTrack)
    //     .def("getTrack", &Recorder::getTrack)
    //     .def("reset", &Recorder::reset)
    //     .def("setPosition", &Recorder::setPosition)
    //     .def("add", &Recorder::add)
    //     ;

    // class_<Player>("Player", init<Track*>())
    //     .def("setTrack", &Player::setTrack)
    //     .def("getTrack", &Player::getTrack)
    //     .def("getTimer", &Player::getTimer)
    //     .def("reset", &Player::reset)
    //     .def("setPosition", &Player::setPosition)
    //     .def("read", &Player::read)
    //     .def("getSpeed", &Player::getSpeed)
    //     .def("setSpeed", &Player::setSpeed)
    //     // TODO setPlaybackMode
    //     // TODO getPlaybackMode
    //     ;
}

