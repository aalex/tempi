#include <iostream>
#include <cstdarg>

void Print(const char* format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    for (int i = 0; format[i] != '\0'; ++i)
    {
        switch (format[i])
        {
            case 'f':
            {
                float value = va_arg(arguments, double);
                std::cout << "Caught a float : " << value;
                break;
            }
            case 'i':
            {
                int value = va_arg(arguments, int);
                std::cout << "Caught an integer : " << value;
                break;
            }
            case 'b':
            {
                bool value = (bool) va_arg(arguments, int);
                std::cout << "Caught a bool : " << value;
                break;
            }
            case 's':
            {
                const char *value = va_arg(arguments, const char*);
                std::cout << "Caught a string : " << value;
                break;
            }
            case 'd':
            {
                double value = va_arg(arguments, double);
                std::cout << "Caught a double : " << value;
                break;
            }
            case 'c':
            {
                char value = (char) va_arg(arguments, int);
                std::cout << "Caught a char : " <<  value;
                break;
            }
            case 'l':
            {
                long long int value = va_arg(arguments, long long int);
                std::cout << "Caught a long long :" << value;
                break;
            }
            default:
                std::cout << "Unsupported: " << format[i];
                break;
        }
        std::cout << std::endl;
    }
    va_end(arguments);
}

int main()
{
    Print("ifbslcdx", 1, 3.14159, true, "blah", 123123123123, 'a', 2.0000000, 2);
    return 0;
}

