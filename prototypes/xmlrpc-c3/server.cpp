/* A simple standalone XML-RPC server based on Abyss that contains a
   simple one-thread request processing loop.  

   xmlrpc_sample_add_server.cpp is a server that does the same thing, but
   does it by running a full Abyss daemon in the background, so it has
   less control over how the requests are served.
*/

#include <cassert>
#include <tempi/node.h>
#include <tempi/message.h>
#include <tempi/graph.h>
#include <tempi/threadedscheduler.h>
#include <iostream>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>

class App
{
    public:
        tempi::ThreadedScheduler::ptr engine_;
        tempi::Graph::ptr graph_;
};

static App app;

const char * const METRO_CLASS = "base.time.metro";
const char * const METRO_INTERVAL_ATTR = "interval";
const char * const METRO_RUNNING_ATTR = "running";
const char * const PRINT_CLASS = "base.flow.print";
const char * const METRO_NODE = "metro0";
const char * const PRINT_NODE = "print0";
const char * const METRO_OUTLET = "0";
const char * const PRINT_INLET = "0";

class SampleAddMethod : public xmlrpc_c::method
{
    public:
        SampleAddMethod()
        {
            // signature and help strings are documentation -- the client
            // can query this information with a system.methodSignature and
            // system.methodHelp RPC.
            this->_signature = "i:ii";  // method's arguments, result are integers
            this->_help = "This method adds two integers together";
        }
        void execute(
            xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *const retvalP)
        {
            int const addend(paramList.getInt(0));
            int const adder(paramList.getInt(1));
            paramList.verifyEnd(2);
            *retvalP = xmlrpc_c::value_int(addend + adder);
        }
};

class TempoMethod : public xmlrpc_c::method
{
    public:
        TempoMethod()
        {
            // signature and help strings are documentation -- the client
            // can query this information with a system.methodSignature and
            // system.methodHelp RPC.
            this->_signature = "i:i";  // method's arguments, result are integers
            this->_help = "This method sets the tempo";
        }
        void execute(
            xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *const retvalP)
        {
            int const tempo(paramList.getInt(0));
            paramList.verifyEnd(1);

            tempi::Node::ptr node = app.graph_->getNode(METRO_NODE);
            node->setAttributeValue(METRO_INTERVAL_ATTR, tempi::Message("i", tempo));

            *retvalP = xmlrpc_c::value_int(0);
        }
};

int main(int const, const char ** const)
{
    {
        app.engine_.reset(new tempi::ThreadedScheduler);
        app.engine_->start(5);
        tempi::ScopedLock::ptr lock = app.engine_->acquireLock();
        app.engine_->createGraph("graph0");
        app.graph_ = app.engine_->getGraph("graph0");
        app.graph_->tick(); // FIXME
        app.graph_->addNode(METRO_CLASS, METRO_NODE);

        app.graph_->addNode(PRINT_CLASS, PRINT_NODE);
        app.graph_->connect(METRO_NODE, METRO_OUTLET, PRINT_NODE, PRINT_INLET);

        tempi::Node::ptr node = app.graph_->getNode(METRO_NODE);
        node->setAttributeValue(METRO_INTERVAL_ATTR, tempi::Message("i", 1000));
        node->setAttributeValue(METRO_RUNNING_ATTR, tempi::Message("b", true));
    }

    try
    {
        xmlrpc_c::registry myRegistry;

        xmlrpc_c::methodPtr const sampleAddMethodP(new SampleAddMethod);
        myRegistry.addMethod("sample.add", sampleAddMethodP);

        xmlrpc_c::methodPtr const tempoMethodP(new TempoMethod);
        myRegistry.addMethod("metro.tempo", tempoMethodP);

        xmlrpc_c::serverAbyss myAbyssServer(
            xmlrpc_c::serverAbyss::constrOpt()
                .registryP(&myRegistry)
                .portNumber(8080)
                .logFileName("/tmp/tempi_xmlrpc_log"));

        while (true)
        {
            std::cout << "Waiting for next RPC..." << std::endl;
            myAbyssServer.runOnce();
            /* This waits for the next connection, accepts it, reads the
               HTTP POST request, executes the indicated RPC, and closes
               the connection.
            */
        }
    }
    catch (std::exception const& e)
    {
        std::cerr << "Something failed.  " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

