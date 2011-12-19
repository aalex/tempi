#include <iostream>

class Foo
{
    public:
        void hello();
    private:
        static const char * const EGG;
        static const char * const BACON;
        static const char * const SPAM;
        static const char * const HAM;
};
const char * const Foo::EGG = "egg\n"
    "etc...\n"
    "etc...\n"
    "etc...\n";
const char * const Foo::BACON = "bacon";
const char * const Foo::SPAM = "spam";
const char * const Foo::HAM = "ham";

void Foo::hello()
{
    std::cout << Foo::EGG;
    std::cout << Foo::BACON;
    std::cout << Foo::SPAM;
    std::cout << Foo::HAM;
}

bool check_something()
{
    Foo f;
    f.hello();
}

int main(int argc, char *argv[])
{
    if (! check_something())
        return 1;
    return 0;
}

