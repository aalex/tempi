#!/bin/bash
sed -i $1 \
    tempi/base/anynode.cpp \
    tempi/base/appsinknode.cpp \
    tempi/base/counternode.cpp \
    tempi/base/delaynode.cpp \
    tempi/base/metro_node.cpp \
    tempi/base/nop_node.cpp \
    tempi/base/prependnode.cpp \
    tempi/base/print_node.cpp \
    tempi/base/receivenode.cpp \
    tempi/base/routenode.cpp \
    tempi/base/spigotnode.cpp \
    tempi/clutter/clutterstagenode.cpp \
    tempi/mapping/filter.cpp \
    tempi/mapping/slidefilter.cpp \
    tempi/midi/midireceivernode.cpp \
    tempi/midi/midiroutenode.cpp \
    tempi/midi/midisendernode.cpp \
    tempi/osc/oscreceivernode.cpp \
    tempi/osc/oscsendernode.cpp \
    tempi/sampler/samplernode.cpp
