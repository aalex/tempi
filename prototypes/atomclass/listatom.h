#ifndef __LIST_ATOM_H__
#define __LIST_ATOM_H__

#include <sstream>
#include <vector>

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

#endif

