#!/bin/bash
g++ `pkg-config --cflags --libs libmapper-0` -Wall -Werror -o run mapper.cpp

