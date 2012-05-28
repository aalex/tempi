#include "tempi/namedobject.h"
#include "tempi/namedobjectmap.h"

using namespace tempi;

class Foo : public NamedObject
{
    public:
        //typedef std::tr1::shared_ptr<Foo> ptr;
        Foo(const char *name) :
            NamedObject(name),
            x_(2)
        {}
        int getX() const
        {
            return x_;
        }
        virtual ~Foo() {} // makes the class polymorphic
    private:
        int x_;
};



int main(int argc, char *argv[])
{
    NamedObjectMap map;
    map.add(NamedObject::ptr(new Foo("foo0")));
    map.add(NamedObject::ptr(new Foo("foo1")));
    if (! map.has("foo0"))
        std::cout << "no foo0 in map\n";
    NamedObject* foo0_named = map.get("foo0").get();
    Foo* foo0_ptr = dynamic_cast<Foo*>(foo0_named);
    std::cout << "x = " << foo0_ptr->getX() << std::endl;
    return 0;
}

