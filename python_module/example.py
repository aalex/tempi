#!/usr/bin/env python
"""
Simple Tempi example
"""

import tempi
import time

VERBOSE = True

def print_info(engine):
    # TODO: (not working yet)
    # print(engine.listNodes("graph0"))
    pass

def run():
    engine = tempi.Wrapper()
    if VERBOSE:
        engine.setLogLevel("DEBUG")
    engine.createGraph("graph0")

    # create nodes:
    engine.createNode("graph0", "base.metro", "metro0")
    engine.createNode("graph0", "base.counter", "counter0")
    engine.createNode("graph0", "base.print", "print0")
    # set metro0's attributes:
    interval = tempi.Message()
    interval.appendInt(100)
    engine.setNodeAttributeValue("graph0", "metro0", "interval", interval)
    running = tempi.Message()
    running.appendBoolean(True)
    engine.setNodeAttributeValue("graph0", "metro0", "running", running)
    # connect nodes:
    engine.connect("graph0", "metro0", "0", "counter0", "0")
    engine.connect("graph0", "counter0", "0", "print0", "0")

    print_info(engine)
    try:
        while True:
            time.sleep(0.1)
    except KeyboardInterrupt, e:
        print("")
    engine.saveGraph("graph0", "/tmp/tempi-example-python-graph0.xml")

if __name__ == "__main__":
    run()

