/*
 * Arduino-serial
 * --------------
 * 
 * A simple command-line example program showing how a computer can
 * communicate with an Arduino board. Works on any POSIX system (Mac/Unix/PC) 
 *
 *
 * Compile with something like:
 * gcc -o arduino-serial arduino-serial.c
 *
 * Created 5 December 2006
 * Copyleft (c) 2006, Tod E. Kurt, tod@todbot.com
 * http://todbot.com/blog/
 *
 */

#include <stdio.h>    /* Standard input/output definitions */
#include <stdlib.h> 
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <getopt.h>

int serialport_init(const char* serialport, int baud);
int serialport_write(int fd, const char* str);
int serialport_read_until(int fd, char* buf, char until);

void read_line(int fd)
{
    char buf[256];
    memset(buf, 0, 256);
    serialport_read_until(fd, buf, '\r');
    printf("read: %s\n", buf);
}

void write_line(int fd, const char *text)
{
    int ret = serialport_write(fd, text);
    if (ret == -1)
    {
        printf("Error writing %s\n", text);
    }
}

void read_some(int fd, char *buf)
{
    
}

int main(int argc, char *argv[]) 
{
    int fd = serialport_init("/dev/ttyACM0", 9600);
    //char GLOBAL_buf[256];

    read_line(fd);
    int count = 0;
    while (1)
    {
        count++;
        usleep(50 * 1000);

        if (count > 20)
            write_line(fd, "ping\r");

        read_line(fd);
    }
    exit(EXIT_SUCCESS);
}
    
int serialport_write(int fd, const char* str)
{
    int len = strlen(str);
    int n = write(fd, str, len);
    if( n != len ) 
        return -1;
    return 0;
}

int serialport_read_until(int fd, char* buf, char until)
{
    char b[1];
    int i = 0;
    do { 
        int n = read(fd, b, 1);  // read a char at a time
        if ( n == -1)
            return -1;    // couldn't read
        if (n == 0)
        {
            usleep(10 * 1000); // wait 10 msec try again
            continue;
        }
        buf[i] = b[0];
        i++;
    }
    while (b[0] != until);

    buf[i] = 0;  // null terminate the string
    return 0;
}

/**
 * Returns the number of bytes read, and put into buf.
 * Zeroes the buffer beforehand.
 */
int serialport_read_some(int fd, char* buf, int max_size)
{
    char b[1];
    memset(buf, 0, sizeof(buf));
    int num_read;
    int stop_at = sizeof(buf);
    for (num_read = 0; num_read < stop_at; num_read++)
    {
        int n = read(fd, b, 1); // read a char at a time
        if (n == -1) // could not read
        {
            num_read = 0;
            break;
        }
        else if (n == 0)
        {
            if (num_read > 0)
                num_read = num_read - 1;
            break;
        }
        else
            buf[num_read] = b[0];
    }
    buf[num_read + 1] = 0; // null terminate the string
    return num_read;
}

// takes the string name of the serial port (e.g. "/dev/tty.usbserial","COM1")
// and a baud rate (bps) and connects to that port at that speed and 8N1.
// opens the port in fully raw mode so you can send binary data.
// returns valid fd, or -1 on error
int serialport_init(const char* serialport, int baud)
{
    struct termios toptions;
    int fd;
    
    //fprintf(stderr,"init_serialport: opening port %s @ %d bps\n",
    //        serialport,baud);

    fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)  {
        perror("init_serialport: Unable to open port ");
        return -1;
    }
    
    if (tcgetattr(fd, &toptions) < 0) {
        perror("init_serialport: Couldn't get term attributes");
        return -1;
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
    
    if (tcsetattr(fd, TCSANOW, &toptions) < 0)
    {
        perror("init_serialport: Couldn't set term attributes");
        return -1;
    }

    return fd;
}

