#include <iostream>

typedef void * Pointer;

class Obj
{
    public:
        Obj(int x) :
            x_(x)
        {}
        void hello()
        {
            std::cout << x_ << std::endl;
        }
    private:
        int x_;
};

void foo(Pointer p)
{
    Obj *o = (Obj *) p;
    o->hello();
}

int main(int argc, char *argv[])
{
    Obj o(2);
    foo((Pointer) &o);
    return 0;
}

