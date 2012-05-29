#!/bin/bash
g++ `pkg-config --libs --cflags libserial` -o run main.cpp
