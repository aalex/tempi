LIBS = liblo
CFLAGS = -Wall -Werror -O2 `pkg-config --cflags $(LIBS)`
LDFLAGS = `pkg-config --libs $(LIBS)`

all: blobsender blobreceiver check_array

blobreceiver: blobreceiver.cpp
	g++ $(CFLAGS) $(LDFLAGS) -o blobreceiver blobreceiver.cpp

blobsender: blobsender.cpp
	g++ $(CFLAGS) $(LDFLAGS) -o blobsender blobsender.cpp

check_array: check_array.cpp
	g++ $(CFLAGS) $(LDFLAGS) -o check_array check_array.cpp

