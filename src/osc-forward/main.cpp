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
 * @file A OSC forwarder.
 */

#include "tempi/base/nop_node.h"
#include "tempi/config.h"
#include "tempi/graph.h"
#include "tempi/message.h"
#include "tempi/node.h"
#include "tempi/osc/oscreceivernode.h"
#include "tempi/osc/oscsendernode.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <glib.h>
#include <iostream>
#include <sstream>

namespace po = boost::program_options;
using namespace tempi;

static const char *PROGRAM_NAME = "osc-forward";

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
        bool addSender(const char *host, unsigned int port);
        bool addReceiver(unsigned int port);
        bool parseHostPort(const std::string &hostPort, std::string &host, unsigned int &port) const;
        bool isValidHost(const std::string &host) const;
        void setupGraph();
    private:
        bool verbose_;
        int num_senders_;
        int num_receivers_;;
        tempi::Graph::ptr graph_;
};

App::App() :
    verbose_(false),
    num_senders_(0),
    num_receivers_(0)
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

bool App::parseHostPort(const std::string &hostPort, std::string &host, unsigned int &port) const
{
    //std::cout << "App::" << __FUNCTION__ << std::endl;
    std::vector<std::string> tokens;
    boost::split(tokens, hostPort, boost::is_any_of(":"));
    if (tokens.size() != 2)
    {
        std::cerr << "Expect exactly one \":\" character in host:port, but got " << hostPort << std::endl;
        return false;
    }
    host = tokens[0];
    try
    {
        port = boost::lexical_cast<unsigned int>(tokens[1]);
    }
    catch (const boost::bad_lexical_cast &e)
    {
        std::cerr << "Could not parse int from " << tokens[1] << ": ";
        std::cerr << e.what();
        return false;
    }
    catch (const boost::bad_any_cast &e)
    {
        std::cerr << "Could not parse int from " << tokens[1] << ": ";
        std::cerr << e.what();
        return false;
    }
    return true;
}

bool App::isValidHost(const std::string &host) const
{
    // TODO
    return true;
}

void App::setupGraph()
{
    NodeFactory::ptr factory(new NodeFactory);

    factory->registerTypeT<osc::OscSenderNode>("osc.send");
    factory->registerTypeT<osc::OscReceiverNode>("osc.receive");
    factory->registerTypeT<base::NopNode>("nop");

    graph_.reset(new tempi::Graph(factory));
    
    graph_->addNode("nop", "nop0");
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
bool App::addSender(const char *host, unsigned int port)
{
    if (verbose_)
        std::cout << "App." << __FUNCTION__ << "(" << host << ", " << port << ")" << std::endl;
    std::ostringstream os;
    os << "send" << num_senders_;
    ++num_senders_;
    std::string name = os.str();

    graph_->addNode("osc.send", name.c_str());
    Node::ptr node = graph_->getNode(name.c_str());
    Message mess = Message("si", host, port);
    node->setProperty("host_port", mess);

    graph_->connect("nop0", 0, name.c_str(), 0);
    return true;
}
bool App::addReceiver(unsigned int port)
{
    if (verbose_)
        std::cout << "App." << __FUNCTION__ << "(" << port << ")" << std::endl;
    std::ostringstream os;
    os << "recv" << num_receivers_;
    ++num_receivers_;
    std::string name = os.str();

    graph_->addNode("osc.receive", name.c_str());
    Node::ptr node = graph_->getNode(name.c_str());
    Message mess = Message("i", port);
    node->setProperty("port", mess);

    graph_->connect(name.c_str(), 0, "nop0", 0);
    return true;
}

struct HostPort
{
    std::string host_;
    unsigned int port_;
};

int App::parse_options(int argc, char **argv)
{
    po::options_description desc("Options");
    desc.add_options()
        ("help,h", "Show this help message and exit")
        ("version", "Show program's version number and exit")
        ("verbose,v", po::bool_switch(), "Enables a verbose output")
        ("receive,r", po::value<std::vector<unsigned int> >(), "Adds a listening OSC port [1024,65535]")
        ("send,s", po::value<std::vector<std::string> >(), "Adds a sending OSC host and port [1024,65535] in the form host:port")
        ;
    po::variables_map options;
    po::store(po::parse_command_line(argc, argv, desc), options);
    po::notify(options);
    
    verbose_ = options["verbose"].as<bool>();
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
    // Options that leaves the App alive:
    if (options.count("receive"))
    {
        std::vector<unsigned int> ports  = options["receive"].as<std::vector<unsigned int> >();
        std::vector<unsigned int>::iterator iter;
        for (iter = ports.begin(); iter != ports.end(); ++iter)
        {
            unsigned int port = (*iter);
            if (port > 65535)
            {
                std::cerr << "OSC receiving Port number too high: " << port << std::endl;
                return 1;
            }
            else
                addReceiver(port);
        }
    }
    if (options.count("send"))
    {
        std::vector<std::string> urls = options["send"].as<std::vector<std::string> >();
        std::vector<std::string>::iterator iter;
        for (iter = urls.begin(); iter != urls.end(); ++iter)
        {
            std::string hostPort = (*iter);
            std::string host;
            unsigned int port;
            if (! parseHostPort(hostPort, host, port))
            {
                std::cerr << "Could not parse host:port: " << hostPort << std::endl;
                return 1;
            }
            else if (port > 65535)
            {
                std::cerr << "OSC receiving Port number too high: " << port << std::endl;
                return 1;
            }
            else if (! isValidHost(host))
            {
                std::cerr << "Invalid host: " << host << std::endl;
                return 1;
            }
            else
                addSender(host.c_str(), port);
        }
    }
    if (verbose_)
        std::cout << "App.graph_: " << (*graph_.get()) << std::endl;
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

