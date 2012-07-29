#include <iostream>
#include <sstream>
#include <tr1/memory>
#include <vector>
#include <boost/lexical_cast.hpp>

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


class IntAtom: public Atom
{
    public:
        IntAtom() {}
        IntAtom(int value) :
            value_(value)
        {}
        void setInt(int value)
        {
            value_ = value;
        }
        int getInt()
        {
            return value_;
        }
    private:
        int value_;
        virtual char doGetName()
        {
            return 'i';
        }
        virtual void doToJSON(std::string &result)
        {
            std::ostringstream os;
            os << value_;
            result = os.str();
        }
        virtual void doFromJSON(const char *json)
        {
            value_ = boost::lexical_cast<int>(json);
        }
};

int main(int argc, char *argv[])
{
    std::vector<Atom::ptr> atoms;
    atoms.push_back(Atom::ptr(new IntAtom(2)));
    std::string json;
    atoms[0].get()->toJSON(json);
    std::cout << "to json: " << json;
    atoms[0].get()->fromJSON("3");
    atoms[0].get()->toJSON(json);
    std::cout << "to json: " << json << std::endl;
    return 0;
}
