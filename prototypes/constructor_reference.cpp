#include <iostream>

class Foo
{
    public:
        Foo(int x) :
            x_(x)
        {
            std::cout << __FUNCTION__ << std::endl;
        }
        ~Foo()
        {
            std::cout << __FUNCTION__ << std::endl;
        }
        int getX() const
        {
            return x_;
        }
    private:
        int x_;
};

static void doSomethingSilly(const Foo &foo)
{
    std::cout << "Foo.x = " << foo.getX() << std::endl;
}

int main(int argc, char *argv[])
{
    std::cout << "This works:" << std::endl;
    Foo foo(2);
    doSomethingSilly(foo);
    std::cout << "This crashes:" << std::endl;
    doSomethingSilly(Foo(2));
    return 0;
}

