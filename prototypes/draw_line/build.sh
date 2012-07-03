#!/bin/bash
gcc -o proto proto.c `pkg-config --libs --cflags clutter-1.0`

