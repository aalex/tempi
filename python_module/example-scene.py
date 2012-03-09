#!/usr/bin/env python
import tempy

def run():
    g = tempiutils.Graph()
    g.node("base.metro", "metro0")
    g.node("base.counter", "count0")
    g.node("base.print", "print0")
    g.connect("metro0", "0", "count0", "0")
    g.connect("count0", "0", "print0", "0")
    g.attr("metro0", "running", tempiutils.mess(True))
    g.runUntilControlC()

if __name__ == "__main__":
    run()

