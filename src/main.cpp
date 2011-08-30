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

#include <clutter/clutter.h>
#include <iostream>
#include "tempi/tempi.h"
#include "tempi/timer.h"
#include "sampler.h"
#include <unistd.h>

void sleep_one_second()
{
    sleep(1); // 1 second
}

int main(int argc, char *argv[])
{
    tempi::hello();
    tempi::Sampler sampler;
    sampler.add(0.0, 0.0);
    std::cout << "add 0,0" << std::endl;
    sleep_one_second();
    sampler.add(1.0, 1.0);
    std::cout << "add 1,1" << std::endl;
    sleep_one_second();
    sampler.add(2.0, 2.0);
    std::cout << "add 2,2" << std::endl;

    sampler.print();

    while (true)
    {
        tempi::ff value = sampler.readLoop();
        std::cout << "Read " << value.get<0>() << ", " << value.get<1>() << std::endl;
        sleep_one_second();
    }
    return 0;
}

