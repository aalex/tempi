#include <iostream>
#include "tempi/tempi.h"

using namespace tempi;

bool check_table()
{
    Table table;
    if (table.getSize() != 0)
    {
        std::cout << "Size should be 0." << std::endl;
        return false;
    }

    Message m;
    m.appendFloat(0.0f);
    table.append(m);
    if (table.getSize() != 1)
    {
        std::cout << "Size should be 1." << std::endl;
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    if (not check_table())
        return 1;
    return 0;
}

