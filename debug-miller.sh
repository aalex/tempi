#!/bin/bash
# This file is not included in the distributed package, nor installed
# It is only useful for developers.
cd $(dirname $0)
CLUTTER_VBLANK=none libtool --mode=execute gdb --args ./src/miller/miller $@

