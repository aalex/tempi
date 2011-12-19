all: midiprobe

midiprobe: midiprobe.cpp
	g++ -pthread -D__LINUX_ALSA__ -D__UNIX_JACK__ -D__LITTLE_ENDIAN__ -o midiprobe -lstk midiprobe.cpp

