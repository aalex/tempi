#!/bin/bash
g++ -Wall -I.. -I"/usr/include/python2.6" `pkg-config --cflags tempi-0.1` -c -o python-tempi.o python-interface.cpp
g++ -o tempi.so -shared --start-group python-tempi.o `pkg-config --libs tempi-0.1` -lboost_python -lutil -lpthread --end-group

