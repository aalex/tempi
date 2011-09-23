#include <iostream>
//#include <libusb-1.0/libusb.h>

#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <linux/input.h>
#include <stdio.h>


using namespace std;


// int main()
// { 
//     /* For some reason, libusb-1.0 does not want to access usb devices this way... */
//     libusb_device **devs;  
//     libusb_context *ctx = NULL;
//     int r;
//     ssize_t cnt; // holds number of devices in list
//     r = libusb_init(&ctx);
//     cout << "Found "<<r<< " devices"<<endl;
//     libusb_exit(ctx);
//     return 0;
// }


int main()
{
    cout << "Device list:" << endl;
    int i, fd;
    char device_output_string[256] = "Unknown";
    char dev_handle_name[FILENAME_MAX] = "/dev/input/event0";
    for (i = 0; i < 256; ++i)
    {
	snprintf(dev_handle_name, FILENAME_MAX, "/dev/input/event%d", i);
	/* open the device read only non-exclusive */
	fd = open(dev_handle_name, O_RDONLY | O_NONBLOCK);
	/* check if the device open */
	if(fd < 0)
	{
	    fd = -1;
	}
	else
	{
	    /* Get name of the device*/
	    ioctl(fd, EVIOCGNAME(sizeof(device_output_string)), device_output_string);
	    cout << i <<" : "<<device_output_string<<" : "<<dev_handle_name<<endl;
	    close(fd);
	    }
    }
    return 0;
}
