OPENCV_LIBS = `pkg-config --libs opencv`
OPENCV_CFLAGS = `pkg-config --cflags opencv`
FREENECT_LIBS = -L/usr/local/lib64 -lfreenect -lfreenect_sync 
FREENECT_CFLAGS = -I/usr/local/include/libfreenect

all: freedraw

freedraw: freedraw.cpp
	g++ -Wall -g $(FREENECT_CFLAGS) $(FREENECT_LIBS) $(OPENCV_CFLAGS) $(OPENCV_LIBS) freedraw.cpp libfreenect_cv.cpp -o freedraw

clean:
	rm -f freedraw *.o

