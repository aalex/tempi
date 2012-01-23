#include "array.h"
#include <iostream>

using namespace tempi;

int main(int argc, char *argv[])
{
    typedef uint8_t Data;
    typedef Array<Data>::index_t Index;

    Array<Data>  array;

    array.resize(4);
    array.setElement((Index) 0, (Data) 'a');
    array.setElement((Index) 1, (Data) 'b');
    array.setElement((Index) 2, (Data) 'c');
    array.setElement((Index) 3, (Data) 'd');
    // should cause an error:
    bool ok = array.setElement((Index) 4, (Data) 'b');
    if (ok)
        std::cout << "setElement(4, ...) but size is 4\n";
    array.print();
    return 0;
}

