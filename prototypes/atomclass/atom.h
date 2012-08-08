#ifndef __ATOM_H__
#define __ATOM_H__

#include <tr1/memory>
#include <string>

class Atom
{
    public:
        typedef std::tr1::shared_ptr<Atom> ptr;
        char getName();
        void toJSON(std::string &result);
        void fromJSON(const char *json);
    private:
        virtual char doGetName() = 0;
        virtual void doToJSON(std::string &result) = 0;
        virtual void doFromJSON(const char *json) = 0;
};

char Atom::getName()
{
    return this->doGetName();
}

void Atom::toJSON(std::string &result)
{
    this->doToJSON(result);
}

void Atom::fromJSON(const char *json)
{
    this->doFromJSON(json);
}

#endif

