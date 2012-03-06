#!/bin/bash
g++ -Wall -c -o abstractfactory.o abstractfactory.cpp
g++ -Wall -c -o main.o main.cpp
g++ -Wall -o run main.o abstractfactory.o

