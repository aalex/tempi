/**
 * The SerialDevice class.
 */

#ifndef __TEMPI_SERIALDEVICE__ // FIXME
#define __TEMPI_SERIALDEVICE__

#include <string>

/**
 * A Serial communication device.
 */
class SerialDevice
{
    public:
        SerialDevice(const char *serial_port, int baud_rate);
        ~SerialDevice();
        bool writeBlob(const char *data);
        //bool readUntil(char *result, size_t max_length, int timeout_ms, char until_char);
        // max_length must be in the range [0, 255] on UNIX
        bool readUntil(char *result, size_t max_length, size_t &total_num_read, unsigned long long timeout_ms, char until_char);
        // bool read(char *result, size_t max_length, int timeout_ms);
        bool closeDevice();
        bool openDevice();
        bool isOpen() const;
        void sleep_ms(unsigned long long ms) const;
    private:
        int fd_;
        bool is_open_;
        std::string serial_port_filename_;
        int baud_rate_;
        bool init_serialport(const char *serialport, int baud);
};

#endif // ifndef

