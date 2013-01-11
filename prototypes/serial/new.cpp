#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int set_interface_attribs(int fd, int speed, int parity)
{
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) != 0)
    {
        printf("error %d from tcgetattr", errno);
        return -1;
    }
    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK; // ignore break signal
    tty.c_lflag = 0;        // no signaling chars, no echo, no canonical processing
    tty.c_oflag = 0;        // no remapping, no delays
    tty.c_cc[VMIN]  = 0;    // read doesn't block
    tty.c_cc[VTIME] = 5;    // 0.5 seconds read timeout
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls, enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB; // one stop bit, not two
    tty.c_cflag &= ~CRTSCTS;
    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
        printf("error %d from tcsetattr", errno);
        return -1;
    }
    return 0;
}

void set_blocking(int fd, int should_block)
{
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) != 0)
    {
        printf("error %d from tggetattr", errno);
        return;
    }
    tty.c_cc[VMIN]  = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 1;            // 0.1 seconds read timeout (example was 0.5)
    if (tcsetattr(fd, TCSANOW, &tty) != 0)
        printf ("error %d setting term attributes", errno);
}

static const char MSG_SEP = 13; //'\r'; // CR

bool writeMessenger(int fd, const std::string &value)
{
    std::ostringstream os;
    std::cout << "SEND: " << value << std::endl;
    os << value << MSG_SEP;
    write(fd, os.str().c_str(), os.str().size());
    return true;
}

int main(int argc, char *argv[])
{
    std::string portname = "/dev/ttyACM0";
    if (argc == 2)
    {
        portname = argv[1];
        std::cout << "Using port name " << portname << std::endl;
    }
    //const int speed = B115200;
    const int speed = 9600; // 115200;

    // TODO: check if have r/w permissions to that file
    std::cout << "OPEN " << portname << std::endl;
    int fd = open(portname.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        printf("error %d opening %s: %s\n", errno, portname.c_str(), strerror (errno));
        return 1;
    }

    set_interface_attribs(fd, speed, 0);  // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking(fd, 0);                // set no blocking

    usleep(50 * 1000); // wait before sending
    
    int led_intensity = 0;

    std::cout << "Message separator is " << (int) MSG_SEP << std::endl;
    std::string received;
    while (1)
    {
        unsigned int ms = 500;
        usleep(ms * 1000);

        std::cout << "------------" << std::endl;

        // Write LED intensity
        std::ostringstream os;
        os << "w O0 " << led_intensity;
        led_intensity = (led_intensity + 100) % 1000;
        writeMessenger(fd, os.str());
        
        // write ping
        std::string ping("ping");
        writeMessenger(fd, ping);

        char buf[100];
        for (int i = 0; i < sizeof buf; i++)
            buf[i] = 0;
        int n = read(fd, buf, sizeof buf);  // read up to 100 characters if ready to read
        if (n == 0)
        {
            std::cout << "RECV: (end of file)\n";
            std::cout << "  buff: \"" << buf << "\"" << std::endl;
        }
        else if (n == -1)
        {
            std::cout << "RECV: ( an ERROR occurred)\n";
            std::cout << "  buff: \"" << buf << "\"" << std::endl;
        }
        else
        {
            std::string the_string(buf);
            std::cout << "RECV: \"" << buf << "\" (" << n << " bytes)" << std::endl;
            if (the_string.find(MSG_SEP) == -1)
            {
                std::cout << " (no separator in line)\n";
                if (received.size() == 0)
                    received = the_string;
                // TODO
                //else
                //    received = 
            }
        }
    }
    std::cout << "TODO CLOSE " << portname << std::endl;
    // TODO cleanly close file descriptor
    return 0;
}

