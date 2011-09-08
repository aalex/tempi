#!/bin/bash
# This file is not included in the distributed package, nor installed
# It is only useful for developers to generate the tempi/types.h file.
cd $(dirname $0)
python utils/gentypes.py > tempi/types.h
