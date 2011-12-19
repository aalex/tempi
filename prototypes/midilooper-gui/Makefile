all: run

run: main.cpp
	g++ -Wall -rdynamic `pkg-config --libs --cflags clutter-1.0 mx-1.0 clutter-1.0` -o run main.cpp

clean:
	rm -rf run

