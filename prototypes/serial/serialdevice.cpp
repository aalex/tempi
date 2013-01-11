#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <tempi/timer.h>

// void read_line(int fd)
// {
//     char buf[256];
//     memset(buf, 0, 256);
//     serialport_read_until(fd, buf, '\r');
//     printf("read: %s\n", buf);
// }
// 
// void write_line(int fd, const char *text)
// {
//     int ret = serialport_write(fd, text);
//     if (ret == -1)
//     {
//         printf("Error writing %s\n", text);
//     }
// }
// 
// void read_some(int fd, char *buf)
// {
//     
// }

//int main(int argc, char *argv[]) 
//{
//    int fd = serialport_init("/dev/ttyACM0", 9600);
//    //char GLOBAL_buf[256];
//
//    read_line(fd);
//    int count = 0;
//    while (1)
//    {
//        count++;
//        usleep(50 * 1000);
//
//        if (count > 20)
//            write_line(fd, "ping\r");
//
//        read_line(fd);
//    }
//    exit(EXIT_SUCCESS);
//}
    
// int serialport_write(int fd, const char* str)
// {
//     int len = strlen(str);
//     int n = write(fd, str, len);
//     if( n != len ) 
//         return -1;
//     return 0;
// }

// int serialport_read_until(int fd, char* buf, char until)
// {
//     char b[1];
//     int i = 0;
//     do { 
//         int n = read(fd, b, 1);  // read a char at a time
//         if ( n == -1)
//             return -1;    // couldn't read
//         if (n == 0)
//         {
//             usleep(10 * 1000); // wait 10 msec try again
//             continue;
//         }
//         buf[i] = b[0];
//         i++;
//     }
//     while (b[0] != until);
// 
//     buf[i] = 0;  // null terminate the string
//     return 0;
// }
// 
// /**
//  * Returns the number of bytes read, and put into buf.
//  * Zeroes the buffer beforehand.
//  */
// int serialport_read_some(int fd, char* buf, int max_size)
// {
//     char b[1];
//     memset(buf, 0, sizeof(buf));
//     int num_read;
//     int stop_at = sizeof(buf);
//     for (num_read = 0; num_read < stop_at; num_read++)
//     {
//         int n = read(fd, b, 1); // read a char at a time
//         if (n == -1) // could not read
//         {
//             num_read = 0;
//             break;
//         }
//         else if (n == 0)
//         {
//             if (num_read > 0)
//                 num_read = num_read - 1;
//             break;
//         }
//         else
//             buf[num_read] = b[0];
//     }
//     buf[num_read + 1] = 0; // null terminate the string
//     return num_read;
// }

void SerialDevice::sleep_ms(unsigned long long ms)
{
    usleep(ms * 1000);
}

bool SerialDevice::readUntil(char *result, size_t max_length, size_t &total_num_read, unsigned long long timeout_ms, char until_char)
{
    bool success = true;
    tempi::Timer timer;
    unsigned long long elapsed;
    char b[1];
    size_t num_read = 0;
    for (num_read = 0; num_read < max_length; num_read++)
    {
        int n = read(fd_, b, 1); // read a char at a time
        if (n == -1) // error. TODO: see errno
        {
            std::cout << __FUNCTION__ << ": Error!";
            std::cout << std::endl;
            success = false;
            break;
        }
        else if (n == 0) // end of file
        {
            usleep(10 * 1000); // wait 10 msec try again
        }
        else
        {
            if (b[0] == until_char)
            {
                break;
            }
            result[num_read] = b[0];
        }
        // check if timeout is reached
        elapsed = tempi::timeposition:to_ms(timer.elapsed());
        if (elapsed > timeout_ms)
        {
            break;
        }
    }
    //if (num_read < max_length)
    //    buf[num_read + 1] = 0;  // null terminate the string

    total_num_read = num_read;
    return success;
}

SerialDevice::SerialDevice(const char *serial_port, int baud_rate)
{
    serial_port_filename_ = serial_port;
    baud_rate_ = baud_rate;
    bool ok = this->openDevice();
}

SerialDevice::~SerialDevice()
{
    if (isOpen())
        closeDevice();
}

bool SerialDevice::isOpen() const
{
    return is_open_;
}

bool SerialDevice::closeDevice()
{
    if (isOpen())
    {
        int result = close(fd_);
        if (result == 0)
        {
            return true;
        }
        else
        {
            std::cout << __FUNCTION__ << ": error. TODO: see errno";
            std::cout << std::endl;
            return false;
        }
    }
    else
    {
        std::cout << __FUNCTION__ << ": Device is not open";
        std::cout << std::endl;
        return false;
    }
}

bool SerialDevice::openDevice()
{
    if (this->isOpen())
    {
        this->close();
    }
    bool ok = this->init_serialport(serial_port_filename_.c_str(), baud_rate_);
    this->is_open_ = ok;
    if (ok)
    {
        std::cout << "Success!\n";
    }
    else
    {
        std::cout << "Failure!\n";
    }
}

/**
 * Takes the string name of the serial port (e.g. "/dev/tty.usbserial","COM1")
 * and a baud rate (BPS) and connects to that port at that speed and 8N1.
 * opens the port in fully raw mode so you can send binary data.
 * returns success or not
 */
bool SerialDevice::init_serialport(const char* serialport, int baud)
{
    struct termios toptions;
    //fprintf(stderr,"init_serialport: opening port %s @ %d bps\n",
    //        serialport,baud);
    fd_ = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd_ == -1)
    {
        perror("init_serialport: Unable to open port ");
        return false;
    }
    if (tcgetattr(fd_, &toptions) < 0)
    {
        perror("init_serialport: Couldn't get term attributes");
        return false;
    }
    speed_t brate = baud; // let you override switch below if needed
    switch (baud)
    {
        case 4800:   brate = B4800;   break;
        case 9600:   brate = B9600;   break;
#ifndef OSNAME_LINUX
        case 14400:  brate = B14400;  break;
#endif
        case 19200:  brate = B19200;  break;
#ifndef OSNAME_LINUX
        case 28800:  brate = B28800;  break;
#endif
        case 38400:  brate = B38400;  break;
        case 57600:  brate = B57600;  break;
        case 115200: brate = B115200; break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);

    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    // no flow control
    toptions.c_cflag &= ~CRTSCTS;

    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw

    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 20;
    
    if (tcsetattr(fd_, TCSANOW, &toptions) < 0)
    {
        perror("init_serialport: Couldn't set term attributes");
        return false;
    }
    return true;
}

