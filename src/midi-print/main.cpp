/*
 * Copyright (C) 2011 Alexandre Quessy
 * 
 * This file is part of Tempi.
 * 
 * Tempi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Tempi.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file A MIDI printer.
 */

#include "tempi/base/print_node.h"
#include "tempi/config.h"
#include "tempi/graph.h"
#include "tempi/message.h"
#include "tempi/midi/midiinput.h"
#include "tempi/midi/midioutput.h"
#include "tempi/midi/midireceivernode.h"
#include "tempi/midi/midisendernode.h"
#include "tempi/node.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <glib.h>
#include <iostream>
#include <sstream>

namespace po = boost::program_options;
using namespace tempi;

static const char *PROGRAM_NAME = "midi-print";

class App
{
    public:
        App();
        /**
         * Return -1 if it's ok to run the program, or retuns 0 or 1 if we should terminate it.
         * Call this begore launching the app.
         */
        int parse_options(int argc, char **argv);
        bool launch();
        void poll();
    protected:
        void setupGraph();
    private:
        bool verbose_;
        unsigned int midi_input_port_;
        unsigned int midi_output_port_;
        tempi::Graph::ptr graph_;
};

App::App() :
    verbose_(false),
    midi_input_port_(0),
    midi_output_port_(0)
{
    setupGraph();
}

void App::poll()
{
    graph_->tick();
    g_usleep(300); // microseconds
    //if (verbose_)
    //    std::cout << ".";
}

void App::setupGraph()
{
    // Register node types
    NodeFactory::ptr factory(new NodeFactory);
    factory->registerTypeT<midi::MidiReceiverNode>("midi.recv");
    factory->registerTypeT<midi::MidiSenderNode>("midi.send");
    factory->registerTypeT<base::PrintNode>("print");

    // Create graph
    graph_.reset(new tempi::Graph(factory));
    
    // MIDI nodes
    graph_->addNode("midi.recv", "midi.recv0");
    graph_->addNode("midi.send", "midi.send0");
    graph_->connect("midi.recv0", 0, "midi.send0", 0);

    // Print stuff
    if (verbose_)
    {
        graph_->addNode("print", "print0");
        graph_->connect("midi.recv0", 0, "print0", 0);
    }

    Message inputMessage("ssi", "set", "port", midi_input_port_);
    graph_->message("midi.recv0", 0, inputMessage);
    Message outputMessage("ssi", "set", "port", midi_output_port_);
    graph_->message("midi.send0", 0, outputMessage);
}

static gboolean on_idle(gpointer data)
{
    App *context = static_cast<App*>(data);
    context->poll();
    return TRUE;
}

bool App::launch()
{
    if (verbose_)
        std::cout << "Running... Press ctrl-C to quit." << std::endl;

    g_idle_add(on_idle, (gpointer) this);
    return true;
}

static void list_midi_devices()
{
    std::cout << "MIDI inputs you can listen to:" << std::endl;
    midi::MidiInput input;
    input.enumerateDevices();
    std::cout << "MIDI outputs you can send to:" << std::endl;
    midi::MidiOutput output;
    output.enumerateDevices();
}

int App::parse_options(int argc, char **argv)
{
    po::options_description desc("Options");
    desc.add_options()
        ("help,h", "Show this help message and exit")
        ("version", "Show program's version number and exit")
        ("list-midi-ports,l", "Lists the MIDI ports")
        ("midi-input-port,i", po::value<unsigned int>()->default_value(0), "Sets the MIDI input port to listen to")
        ("midi-output-port,o", po::value<unsigned int>()->default_value(0), "Sets the MIDI output port to send to")
        ("verbose,v", po::bool_switch(), "Enables a verbose output")
        ;
    po::variables_map options;
    po::store(po::parse_command_line(argc, argv, desc), options);
    po::notify(options);
    
    verbose_ = options["verbose"].as<bool>();
    midi_input_port_ = options["midi-input-port"].as<unsigned int>();
    midi_output_port_ = options["midi-output-port"].as<unsigned int>();
    if (verbose_)
        std::cout << "MIDI input port: " << midi_input_port_ << std::endl;
    // Options that makes the program exit:
    if (options.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }
    if (options.count("version"))
    {
        std::cout << PROGRAM_NAME << " " << PACKAGE_VERSION << std::endl;
        return 0; 
    }
    if (options.count("list-midi-ports"))
    {
        list_midi_devices();
        return 0; 
    }
    return -1;
}

int main(int argc, char *argv[])
{
    App app;
    int ret;
    try
    {
        ret = app.parse_options(argc, argv);
    }
    catch (const boost::bad_any_cast &e)
    {
        std::cerr << "Error parsing options ";
        std::cerr << e.what() << std::endl;
        return 1;
    }
    if (ret != -1)
        return ret;

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    app.launch();
    g_main_loop_run(loop);
    return 0;
}

