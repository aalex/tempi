LDFLAGS = -shared -fPIC -Wl,--export-dynamic
all: hello.so main 
	@echo DONE

#hello.o: hello.cpp
#	g++ -c -fPIC -o hello.o hello.cpp

hello.so: hello.cpp
	g++ -g $(LDFLAGS) -o hello.so hello.cpp
	chmod +x hello.so

main: main.cpp
	g++ -g -ldl -o main main.cpp

clean:
	rm -f main *.so *.o

