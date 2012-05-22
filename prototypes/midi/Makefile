all: midiprobe alsalist

midiprobe: midiprobe.cpp
	g++  -O3 -Wall -DHAVE_GETTIMEOFDAY -D__LINUX_ALSA__ -D__LINUX_ALSASEQ__ -D__LITTLE_ENDIAN__  -o midiprobe midiprobe.cpp -lpthread -lasound -lasound -lm  -lrtmidi

alsalist: alsalist.cpp
	g++ -lasound `pkg-config --libs --cflags glib-2.0` -o alsalist alsalist.cpp

clean:
	rm -rf alsalist midiprobe

