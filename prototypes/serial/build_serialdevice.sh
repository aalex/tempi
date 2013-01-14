#!/bin/bash

# g++ `pkg-config --libs --cflags tempi-0.2` -c -o serialdevice.o serialdevice.cpp
g++ -o check_serialdevice check_serialdevice.cpp serialdevice.cpp `pkg-config --libs --cflags tempi-0.2`  

