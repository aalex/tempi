DEPS=tempi-0.1 shmdata-0.4 shmdata-any-0.4

all: reader writer

reader: reader.cpp
	g++ -o reader reader.cpp -Wall -Werror `pkg-config --libs --cflags ${DEPS}`

writer: writer.cpp
	g++ -o writer writer.cpp -Wall -Werror `pkg-config --libs --cflags ${DEPS}`

