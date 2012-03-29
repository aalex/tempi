#!/usr/bin/env python
import tempy

def run():
    g = tempy.Graph()
    g.node("base.metro", "metro0")
    g.node("base.counter", "count0")
    g.node("base.print", "print0")
    g.connect("metro0", "0", "count0", "0")
    g.connect("count0", "0", "print0", "0")
    g.attr("metro0", "running", tempy.mess(True))
    g.runUntilControlC()
    g.save("example_scene.xml")

if __name__ == "__main__":
    run()

