/*=============================================================================
                        sample_add_client_complex.cpp
===============================================================================
  This is an example of an XML-RPC client that uses XML-RPC for C/C++
  (Xmlrpc-c).

  In particular, it uses the complex lower-level interface that gives you
  lots of flexibility but requires lots of code.  Also see
  xmlrpc_sample_add_server, which does the same thing as this program,
  but with much simpler code because it uses a simpler facility of
  Xmlrpc-c.

  This program actually gains nothing from using the more difficult
  facility.  It is for demonstration purposes.
=============================================================================*/

#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>
#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client.hpp>

int main(int argc, char **)
{
    if (argc - 1 > 0)
    {
        std::cerr << "This program has no arguments" << std::endl;
        exit(1);
    }
    try
    {
        xmlrpc_c::clientXmlTransport_curl myTransport(
            xmlrpc_c::clientXmlTransport_curl::constrOpt()
                .timeout(10000)  // milliseconds
                .user_agent("sample_add/1.0"));

        xmlrpc_c::client_xml myClient(&myTransport);

        // call sample.add 5 7
        xmlrpc_c::paramList sampleAddParms;
        sampleAddParms.add(xmlrpc_c::value_int(5));
        sampleAddParms.add(xmlrpc_c::value_int(7));
        std::string const methodName("sample.add");
        xmlrpc_c::rpcPtr myRpcP(methodName, sampleAddParms);

        // call metro.tempo <value>
        xmlrpc_c::paramList metro_params;
        metro_params.add(xmlrpc_c::value_int(500));
        std::string const metro_methodName("metro.tempo");
        xmlrpc_c::rpcPtr myRpcMetro(metro_methodName, metro_params);


        std::string const serverUrl("http://localhost:8080/RPC2");

        xmlrpc_c::carriageParm_curl0 myCarriageParm(serverUrl);
        myRpcP->call(&myClient, &myCarriageParm);
        myRpcMetro->call(&myClient, &myCarriageParm);

        assert(myRpcMetro->isFinished());
        assert(myRpcP->isFinished());

        int const sum(xmlrpc_c::value_int(myRpcP->getResult()));
        int const whatever(xmlrpc_c::value_int(myRpcMetro->getResult()));
            // Assume the method returned an integer; throws error if not

        std::cout << "Result of RPC (sum of 5 and 7): " << sum << std::endl;

    }
    catch (std::exception const& e)
    {
        std::cerr << "Client threw error: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Client threw unexpected error." << std::endl;
    }
    return 0;
}

