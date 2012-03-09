#!/usr/bin/env python
import tempy

def run():
    g = tempy.Graph()
    g.node("osc.receive", "oscreceive0")
    g.node("osc.route", "route_kinect")
    g.node("base.print", "print0")
    g.attr("oscreceive0", "port", tempy.mess(7110))
    g.attr("route_kinect", "paths", tempy.mess("/user"))
    g.connect("oscreceive0", "incoming", "route_kinect", "incoming")
    g.connect("route_kinect", "/user", "print0", "0")
    g.runUntilControlC()

if __name__ == "__main__":
    run()

