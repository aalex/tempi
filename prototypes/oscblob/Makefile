LIBS = liblo
CFLAGS = -Wall -Werror -O2 `pkg-config --cflags $(LIBS)`
LDFLAGS = `pkg-config --libs $(LIBS)`

all: blobsender blobreceiver check_array proto_armadillo matrix

blobreceiver: blobreceiver.cpp
	g++ $(CFLAGS) $(LDFLAGS) -o blobreceiver blobreceiver.cpp

blobsender: blobsender.cpp
	g++ $(CFLAGS) $(LDFLAGS) -o blobsender blobsender.cpp

check_array: check_array.cpp
	g++ $(CFLAGS) $(LDFLAGS) -o check_array check_array.cpp

proto_armadillo: proto_armadillo.cpp
	g++ proto_armadillo.cpp -o proto_armadillo -O1 -larmadillo

matrix: matrix.cpp
	g++ matrix.cpp -o matrix

clean:
	rm -f blobsender blobreceiver check_array proto_armadillo matrix 
