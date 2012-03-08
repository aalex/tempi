#!/bin/bash
set -o verbose

PORT=12222

# TODO osc-send osc.udp://localhost:$PORT /tempi/rec/select 0
osc-send osc.udp://localhost:$PORT /tempi/rec/start
for i in {1..30}; do osc-send osc.udp://localhost:$PORT /tempi/rec/write `python -c "print(($i % 11) * 10.0)"` `python -c "print(($i % 13) * 10.0)"`; done
osc-send osc.udp://localhost:$PORT /tempi/rec/stop
