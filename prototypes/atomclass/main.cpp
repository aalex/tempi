#include <iostream>
#include <sstream>
#include <tr1/memory>
#include <vector>
#include <boost/lexical_cast.hpp>

/**
 * It would make more sense to refactor the message class so that the
 * handling of each atom type would be in a hierarchy of classes for
 * each atom type. This way, we would get rid of switch/case statements
 * and use pure virtual methods instead. That would help preventing some
 * mistakes at compile time.
 *
 * Also, I think we could simplify the types in tempi so that we keep
 * only the few types supported in JSON. Here they are:
 *
 *  - int
 *  - float
 *  - string
 *  - null
 *  - list
 *  - dict
 */

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

class ListAtom: public Atom
{
    public:
        ListAtom() {}
        void append(Atom::ptr value)
        {
            atoms_.push_back(value);
        }
        void clear()
        {
            atoms_.clear();
        }
        bool set(unsigned int index, Atom::ptr value)
        {
            // TODO: check for bad index
            atoms_[index] = value;
            return true;
        }
        Atom::ptr get(unsigned int index)
        {
            // TODO: check for bad index
            return atoms_[index];
        }
        unsigned int size()
        {
            return atoms_.size();
        }
    private:
        std::vector<Atom::ptr> atoms_;
        virtual char doGetName()
        {
            return 'l';
        }
        virtual void doToJSON(std::string &result)
        {
            std::ostringstream os;
            std::vector<Atom::ptr>::const_iterator iter;
            os << "[";
            for (iter = atoms_.begin(); iter != atoms_.end(); iter++)
            {
                std::string jsonized;
                (*iter)->toJSON(jsonized);
                os << jsonized;
                os << ",";
            }
            os << "]";
            result = os.str();
        }
        virtual void doFromJSON(const char *json)
        {
            // TODO!
            std::cout << __FUNCTION__ << ": not implemented.\n";
        }
};

/**
 * Casts a Atom::ptr into a ListAtom*.
 * TODO: use shared_ptr casters instead.
 */
ListAtom *LIST_ATOM(Atom::ptr atom)
{
    return static_cast<ListAtom*>(atom.get());
}

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
    
    Atom::ptr list = Atom::ptr(new ListAtom());
    LIST_ATOM(list)->append(Atom::ptr(new IntAtom(1)));
    LIST_ATOM(list)->append(Atom::ptr(new IntAtom(2)));
    LIST_ATOM(list)->append(Atom::ptr(new IntAtom(3)));
    Atom::ptr list2 = Atom::ptr(new ListAtom());
    LIST_ATOM(list2)->append(Atom::ptr(new IntAtom(4)));
    LIST_ATOM(list2)->append(Atom::ptr(new IntAtom(5)));
    LIST_ATOM(list)->append(list2);
    LIST_ATOM(list)->toJSON(json);
    std::cout << "to json: " << json << std::endl;
    return 0;
}
