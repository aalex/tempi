#!/usr/bin/env python
"""
Simple Tempi example
"""

import tempi
import time

def run():
    engine = tempi.Wrapper()
    # engine.setLogLevel("INFO")
    engine.createGraph("graph0")
    engine.createNode("graph0", "base.metro", "metro0")
    engine.createNode("graph0", "base.print", "print0")
    interval = tempi.Message()
    interval.appendInt(100)
    engine.setNodeAttributeValue("graph0", "metro0", "interval", interval)
    engine.connect("graph0", "metro0", "0", "print0", "0")
    try:
        while True:
            time.sleep(0.1)
    except KeyboardInterrupt, e:
        print("")

if __name__ == "__main__":
    run()

