#include "serialdevice.h"

void read_a_line(SerialDevice &dev)
{
    char buf[256];
    memset(buf, 0, 256);
    size_t total_read;
    unsigned long long timeout_ms = 50;
    char until_char = '\r';
    dev.readUntil(buf, sizeof(buf), total_read, timeout_ms, until_char);
    printf("read: %s\n", buf);
}

void write_a_line(SerialDevice &dev, const char *text)
{
    dev.write(text);
}

int main(int argc, char *argv[]) 
{
    SerialDevice dev = SerialDevice("/dev/ttyACM0", 9600);

    read_a_line(dev);

    int count = 0;
    while (1)
    {
        count++;
        dev.sleep_ms(10);

        if (count > 20)
            write_a_line(dev, "ping\r");

        read_a_line(dev);
    }
    return 0;
}

