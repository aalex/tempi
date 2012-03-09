#!/bin/bash
CFLAGS = `python-config --cflags` `pkg-config --cflags tempi-0.1` -I.. -enable-lib64 -fPIC -Wall
LIBS = `python-config --libs` `pkg-config --libs tempi-0.1` --enable-lib64 -fPIC -shared --start-group -lutil -lpthread --end-group

ifeq ($(shell uname -s),Darwin)
  LIBS := -lboost_python-mt -L/opt/local/lib
else
  LIBS := -lboost_python-py26
endif

all: tempi.so

tempi.o: python-tempi.cpp
	g++ ${CFLAGS} -c -o python-tempi.o python-tempi.cpp

tempi.so: tempi.o
	g++ ${LIBS} -o tempi.so python-tempi.o

install: tempi.so
	install tempi.so /usr/local/lib/python2.6/dist-packages
	ldconfig

clean:
	rm -f *.so *.o

run: tempi.so
	PYTHONPATH=${PWD} python example.py
