#include <iostream>
#include "tempi/table/table.h"

using namespace tempi;

bool check_table()
{
    table::Table table0;
    if (table0.getSize() != 0)
    {
        std::cout << "Size should be 0." << std::endl;
        return false;
    }

    Message m;
    m.appendFloat(0.0f);
    table0.append(m);
    if (table0.getSize() != 1)
    {
        std::cout << "Size should be 1." << std::endl;
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    if (! check_table())
        return 1;
    return 0;
}

