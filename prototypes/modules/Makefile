LDFLAGS = -shared -fPIC -Wl,--export-dynamic
all: libtempihello.so main 
	@echo DONE

libtempihello.so: hello.cpp
	g++ -g $(LDFLAGS) -o libtempihello.so hello.cpp

main: main.cpp
	g++ -g -ldl -lboost_filesystem-mt -o main main.cpp

clean:
	rm -f main *.so *.o

install: libtempihello.so
	install -D libtempihello.so /usr/local/lib/tempi-0.2

