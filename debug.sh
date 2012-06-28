#!/bin/bash
cd $(dirname $0)
libtool --mode=execute gdb --args ./src/tempi-launch/tempi-launch -v $@

