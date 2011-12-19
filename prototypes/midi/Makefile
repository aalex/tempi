all: midiprobe alsalist

midiprobe: midiprobe.cpp
	g++ -pthread -D__LINUX_ALSA__ -D__UNIX_JACK__ -D__LITTLE_ENDIAN__ -o midiprobe -lstk midiprobe.cpp

alsalist: alsalist.cpp
	g++ -lasound `pkg-config --libs --cflags glib-2.0` -o alsalist alsalist.cpp

clean:
	rm -rf alsalist midiprobe

