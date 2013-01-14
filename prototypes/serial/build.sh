#!/bin/bash
g++ -o run main.cpp `pkg-config --libs --cflags libserial`
g++ -o new new.cpp

