all: client server

client: calcclient.cpp wsdlspec
	g++ -o calcclient++ calcclient.cpp soapC.cpp soapcalcProxy.cpp `pkg-config --cflags --libs gsoap++`

server: calcserver.cpp wsdlspec
	g++ -o calcserver++ calcserver.cpp soapC.cpp soapcalcService.cpp `pkg-config --cflags --libs gsoap++`

wsdlspec: calc.h
	soapcpp2 -i calc.h

clean:
	rm -f calcclient++ calcserver++ *.xml calc.nsmap calc.wsdl soapcalcProxy.cpp soapcalcProxy.h soapcalcService.cpp soapcalcService.h soapStub.h soapC.cpp soapH.h ns.xsd