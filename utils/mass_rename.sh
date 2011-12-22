#!/bin/bash
# Usage: ./utils/mass_rename.sh Foo Bar
# $ git mv tempi/foo.h tempi/bar.h
# $ git mv tempi/foo.cpp tempi/bar.cpp
# Usage: ./utils/mass_rename.sh foo.h bar.h

sed -i "s/$1/$2/g" \
    tempi/*.{h,cpp} \
    tempi/base/*.{h,cpp} \
    tempi/fsm/*.{h,cpp} \
    tempi/mapping/*.{h,cpp} \
    tempi/math/*.{h,cpp} \
    tempi/midi/*.{h,cpp} \
    tempi/osc/*.{h,cpp} \
    tempi/sampler/*.{h,cpp} \
    tempi/table/*.{h,cpp} \
    src/midilooper/*.{h,cpp} \
    src/tempi-draw/*.{h,cpp} \
    src/osc-forward/*.{h,cpp} \
    tests/*.cpp

