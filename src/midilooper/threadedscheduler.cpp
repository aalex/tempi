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
 * @file Some prototype of using the ThreadedScheduler.
 */

#include "tempi/message.h"
#include "tempi/threadedscheduler.h"
#include <iostream>

#define WITH_MAIN 1

using namespace tempi;

#ifdef WITH_MAIN

int main(int argc, char* argv[])
{
    float sleepTime = 25.0f;
    std::cout << "main: startup" << std::endl;
    ThreadedScheduler scheduler;
    scheduler.createGraph("graph0");
    scheduler.start(5); // ms
    scheduler.sleepMilliseconds(sleepTime);
    //scheduler.sendMessage(Message("sif", "hello", 2, 3.14159f));
    scheduler.sendMessage(Message("ssss", "__tempi__", "addNode", "base.receive", "receive0"));
    scheduler.sleepMilliseconds(sleepTime);
    scheduler.sendMessage(Message("ssss", "__tempi__", "addNode", "base.print", "print0"));
    scheduler.sleepMilliseconds(sleepTime);
    std::cout << "main: waiting for thread" << std::endl;
    scheduler.stop();
    std::cout << "main: done" << std::endl;
    return 0;
}

#endif // WITH_MAIN

