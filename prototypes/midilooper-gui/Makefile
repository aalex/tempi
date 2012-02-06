all: run signals_to_messages

run: main.cpp
	g++ -Wall -rdynamic -Wl,--export-dynamic `pkg-config --libs --cflags mx-1.0 clutter-1.0` -o run main.cpp

signals_to_messages: signals_to_messages.cpp
	g++ -Wall -rdynamic -Wl,--export-dynamic `pkg-config --libs --cflags mx-1.0 clutter-1.0 tempi-0.1` -o signals_to_messages signals_to_messages.cpp

clean:
	rm -rf run signals_to_messages

