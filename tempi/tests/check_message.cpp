#include <iostream>
#include <string>
#include "tempi/message.h"

using namespace tempi;

static const bool VERBOSE = false;

bool check_message()
{
    Message m;
    m.appendFloat(3.14159);
    m.appendString("hello");
    m.appendInt(2);

    if (VERBOSE)
    {
        std::cout << "float: " << m.getFloat(0) << std::endl;
        std::cout << "string: " << m.getString(1) << std::endl;
        std::cout << "int: " << m.getInt(2) << std::endl;
        std::cout << "Message: " << m << std::endl;
    }
    return true;
}

static bool fail(const char *message)
{
    std::cout << message << std::endl;
    return false;
}

bool check_valist()
{
    Message m = Message("sif", "blah", 2, 3.14159f);
    if (m.getString(0) != "blah")
    {
        return fail("expected blah");
    }
    if (m.getInt(1) != 2)
    {
        return fail("expected 2");
    }
    if (m.getFloat(2) != 3.14159f)
    {
        return fail("expected 3.14159f");
    }
    return true;
}

bool check_clone()
{
    Message m;
    m.appendFloat(3.14159);
    m.appendString("hello");
    m.appendInt(2);
    Message x = m.cloneRange(1, 2);
    if (x.getSize() != 2)
    {
        std::cout << "Expected message of size 2 but got something else\n";
        return false;
    }
    if (x.getTypes() != "si")
    {
        std::cout << "Expected message with si types\n";
        return false;
    }
    return true;
}

struct Foo
{
    int i;
};

bool checkPointer()
{
    Foo f;
    f.i = 2;

    Message m;
    m.appendPointer((void *) &f);
    if (m.getTypes() != "p")
    {
        std::cout << "Expected message with p types\n";
        return false;
    }
    Foo *p = (Foo *) m.getPointer(0);
    if (p->i != f.i)
    {
        std::cout << "Struct pointed by pointer doesn't match.\n";
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (! check_message())
        return 1;
    if (! check_clone())
        return 1;
    if (! check_valist())
        return 1;
    if (! checkPointer())
        return 1;
    return 0;
}

