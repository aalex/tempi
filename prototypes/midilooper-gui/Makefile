all: run

run: main.cpp
	g++ -Wall -rdynamic -Wl,--export-dynamic `pkg-config --libs --cflags mx-1.0 clutter-1.0` -o run main.cpp

clean:
	rm -rf run

