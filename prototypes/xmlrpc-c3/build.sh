#!/bin/bash
g++ -lxmlrpc_cpp -lxmlrpc_server++ -lxmlrpc_server_abyss++ -lpthread -o server server.cpp
g++ -lxmlrpc_cpp -lxmlrpc_client++ -lpthread -o client client.cpp

