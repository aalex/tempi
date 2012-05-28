#!/usr/bin/env python
import tempy

def run():
    g = tempy.Graph()
    g.set_log_level("INFO")
    # Receive OSC
    g.node("osc.receive", "oscreceive0")
    g.attr("oscreceive0", "port", tempy.mess(7110))

    # Route /joint
    g.node("osc.route", "route_kinect")
    g.attr("route_kinect", "paths", tempy.mess("/joint"))

    g.node("base.print", "print_osc")
    g.attr("print_osc", "prefix", tempy.mess("OSC: ")) 
    g.attr("print_osc", "enabled", tempy.mess(True)) 

    # Route r_hand and r_elbow
    g.node("base.route", "route_joints")
    g.attr("route_joints", "selectors", tempy.mess("r_hand", "r_elbow"))
    
    g.node("base.print", "print_joints")
    g.attr("print_joints", "prefix", tempy.mess("all joints: ")) 
  
    # remove first atom of r_elbow:
    g.node("flow.unpack", "unpack_elbow")
    g.attr("unpack_elbow", "types", tempy.mess("ifff"))
    g.node("flow.pack", "pack_elbow")
    g.attr("pack_elbow", "number", tempy.mess(3))
    g.connect("unpack_elbow", "1", "pack_elbow", "0")
    g.connect("unpack_elbow", "2", "pack_elbow", "1")
    g.connect("unpack_elbow", "3", "pack_elbow", "2")
    g.node("base.print", "print_elbow")
    g.attr("print_elbow", "prefix", tempy.mess("ELBOW: "))
    g.connect("route_joints", "r_elbow", "unpack_elbow", "input")
    g.connect("pack_elbow", "result", "print_elbow", "0")

    # remove first atom of r_hand:
    g.node("flow.unpack", "unpack_hand")
    g.attr("unpack_hand", "types", tempy.mess("ifff"))
    g.node("flow.pack", "pack_hand")
    g.attr("pack_hand", "number", tempy.mess(3))
    g.connect("unpack_hand", "1", "pack_hand", "0")
    g.connect("unpack_hand", "2", "pack_hand", "1")
    g.connect("unpack_hand", "3", "pack_hand", "2")
    g.node("base.print", "print_hand")
    g.attr("print_hand", "prefix", tempy.mess("HAND: "))
    g.connect("route_joints", "r_hand", "unpack_hand", "input")
    g.connect("pack_hand", "result", "print_hand", "0")

    # Connect all nodes together
    g.connect("oscreceive0", "incoming", "route_kinect", "incoming")
    g.connect("oscreceive0", "incoming", "print_osc", "0")
    g.connect("route_kinect", "/joint", "route_joints", "0")
    g.connect("route_kinect", "/joint", "print_joints", "0")

    g.runUntilControlC()
    g.save("osceleton1.xml")

if __name__ == "__main__":
    run()

