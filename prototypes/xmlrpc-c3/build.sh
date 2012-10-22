#!/bin/bash
CLIENTLIBS=`xmlrpc-c-config c++2 client --libs --cflags`
SERVERLIBS=`xmlrpc-c-config c++2 abyss-server --libs --cflags`
TEMPILIBS=`pkg-config --libs --cflags tempi-0.1`

g++ -o server server.cpp $SERVERLIBS $TEMPILIBS -lpthread
g++ -o client client.cpp $CLIENTLIBS $TEMPILIBS -lpthread

