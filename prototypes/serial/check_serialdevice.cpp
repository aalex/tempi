#include "serialdevice.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdio>

static const char LINE_SEP = '\n';

void read_a_line(SerialDevice &dev)
{
    char buf[256];
    memset(buf, 0, 256);
    size_t total_read;
    unsigned long long timeout_ms = 100;
    char until_char = LINE_SEP;
    dev.readUntil(buf, 255, total_read, timeout_ms, until_char);
    std::string tmp_string = std::string(buf);
    if (total_read > 0)
    {
        std::cout << std::endl;
        std::cout << __FUNCTION__ << " did read \"" << tmp_string.substr(0, total_read - 1) << "\"";
        std::cout << " (" << total_read << " bytes)";
        std::cout << std::endl;
    }
    std::cout << ".";
    std::cout.flush();
}

void write_a_line(SerialDevice &dev, const char *text)
{
    std::ostringstream os;
    os << text;
    os << LINE_SEP;
    dev.writeBlob(os.str().c_str());
}

int main(int argc, char *argv[]) 
{
    std::string device_name = std::string("/dev/ttyACM0");
    if (argc == 2)
    {
        device_name = std::string(argv[1]);
    }
    SerialDevice dev = SerialDevice(device_name.c_str(), 9600);

    read_a_line(dev);

    int count = 0;
    while (1)
    {
        count = (count + 1) % 20;
        dev.sleep_ms(10);

        if (count == 19)
            write_a_line(dev, "ping");

        read_a_line(dev);
    }
    return 0;
}

