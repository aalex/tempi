#!/usr/bin/env python
import tempy

def run():
    g = tempy.Graph()
    # RECEIVE OSC
    g.set_log_level("INFO")
    g.node("osc.receive", "oscreceive0")
    g.attr("oscreceive0", "port", tempy.mess(7770))

    # ROUTE OSC
    g.node("osc.route", "route_midi")
    g.attr("route_midi", "paths", tempy.mess("/note", "/control", "/program"))
    g.connect("oscreceive0", "incoming", "route_midi", "incoming")    
    
    # MIDINOTE
    g.node("midi.note", "note0")
    g.connect("route_midi", "/note", "note0", "ints")

    # PRINT MIDINOTE
    g.node("base.print", "print_note")
    g.attr("print_note", "prefix", tempy.mess("Got note:"))
    g.connect("note0", "midi_event", "print_note", "0")

    # SEND MIDI
    g.node("midi.output", "midi_out")
    g.attr("midi_out","port", tempy.mess(0))
    g.connect("note0", "midi_event", "midi_out", "0")
    g.runUntilControlC()

if __name__ == "__main__":
    run()

