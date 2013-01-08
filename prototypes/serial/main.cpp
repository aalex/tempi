#include <SerialStream.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>

void printDefaults()
{
    const LibSerial::SerialStreamBuf::BaudRateEnum baudRate = LibSerial::SerialStreamBuf::DEFAULT_BAUD;
    const LibSerial::SerialStreamBuf::CharSizeEnum charSize = LibSerial::SerialStreamBuf::DEFAULT_CHAR_SIZE;
    const LibSerial::SerialStreamBuf::ParityEnum parityType = LibSerial::SerialStreamBuf::DEFAULT_PARITY;
    const short numOfStopBits = LibSerial::SerialStreamBuf::DEFAULT_NO_OF_STOP_BITS;
    const LibSerial::SerialStreamBuf::FlowControlEnum flowControlType = LibSerial::SerialStreamBuf::DEFAULT_FLOW_CONTROL;
    std::cout << "defaults: " << std::endl;
    std::cout << " * baudRate " << baudRate << std::endl;
    std::cout << " * charSize " << charSize << std::endl;
    std::cout << " * parityType " << parityType << std::endl;
    std::cout << " * numOfStopBits " << numOfStopBits << std::endl;
    std::cout << " * flowControlType " << flowControlType << std::endl;
}

void printIfOpen(const LibSerial::SerialStream &stream)
{
    std::cout << "Is open? ";
    if (stream.IsOpen())
        std::cout << "yes";
    else
        std::cout << "no";
    std::cout << std::endl;
}

void signal_handler(int s)
{
    std::cout << "Caught signal " << s << std::endl;
    exit(0); 
}

int main(int argc, char **argv)
{
    // install signal handler (for Control-C)
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = signal_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    static const char * const DEVICE_NAME = "/dev/ttyACM0";

    printDefaults();

    // Create a SerialStream instance.
    LibSerial::SerialStream serial_stream;
    printIfOpen(serial_stream);

    serial_stream.
    serial_stream.SetVTime(1);
    serial_stream.SetVMin(0);

    // Open the serial port for communication.
    serial_stream.Open(DEVICE_NAME);
    printIfOpen(serial_stream);
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

    while (true)
    {
        
    }

    serial_stream.Close();
    printIfOpen(serial_stream);

    return 0;
}

