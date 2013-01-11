#include "serialdevice.h"
#include <iostream>
#include <cstring>
#include <cstdio>

void read_a_line(SerialDevice &dev)
{
    char buf[256];
    memset(buf, 0, 256);
    size_t total_read;
    unsigned long long timeout_ms = 100;
    char until_char = '\r';
    dev.readUntil(buf, 255, total_read, timeout_ms, until_char);
    if (total_read > 0)
    {
        std::cout << __FUNCTION__ << " did read \"" << buf << "\"";
        std::cout << " (" << total_read << " bytes)";
        std::cout << std::endl;
    }
    std::cout << ".";
    std::cout.flush();
}

void write_a_line(SerialDevice &dev, const char *text)
{
    dev.writeBlob(text);
}

int main(int argc, char *argv[]) 
{
    SerialDevice dev = SerialDevice("/dev/ttyACM0", 9600);

    read_a_line(dev);

    int count = 0;
    while (1)
    {
        count = (count + 1) % 20;
        dev.sleep_ms(10);

        if (count == 19)
            write_a_line(dev, "ping\r");

        read_a_line(dev);
    }
    return 0;
}

