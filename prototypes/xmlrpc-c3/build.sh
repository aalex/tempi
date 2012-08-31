#!/bin/bash
LIBS=`xmlrpc-c-config c++2 abyss-server client --libs --cflags`
# -lxmlrpc_cpp -lxmlrpc_server++ -lxmlrpc_server_abyss++
g++ -o server server.cpp $LIBS -lpthread
g++ -o client client.cpp $LIBS -lpthread

