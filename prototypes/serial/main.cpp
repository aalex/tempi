#include <SerialStream.h>
#include <iostream>

int main(int argc, char **argv)
{
    // Create a SerialStream instance.
    LibSerial::SerialStream serial_stream;
    // Open the serial port for communication.
    serial_stream.Open("/dev/ttyS0") ;
    // The various available baud rates are defined in SerialStreamBuf class. 
    // This is to be changed soon. All serial port settings will be placed in
    // in the SerialPort class.
    serial_stream.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_115200);
    // Use 8 bit wide characters. 
    serial_stream.SetCharSize(LibSerial::SerialStreamBuf::CHAR_SIZE_8);
    // Use one stop bit. 
    serial_stream.SetNumOfStopBits(1);
    // Use odd parity during serial communication. 
    serial_stream.SetParity(LibSerial::SerialStreamBuf::PARITY_ODD);
    // Use hardware flow-control. 
    serial_stream.SetFlowControl(LibSerial::SerialStreamBuf::FLOW_CONTROL_HARD);

    serial_stream << "Hello, Serial Port." << std::endl;

    serial_stream.Close();

    return 0;
}

