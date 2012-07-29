#ifndef __INT_ATOM_H__
#define __INT_ATOM_H__

#include "atom.h"
#include <boost/lexical_cast.hpp>
#include <sstream>

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
/**
 * Casts a Atom::ptr into an IntAtom*.
 * TODO: use shared_ptr casters instead.
 */
IntAtom *INT_ATOM(Atom::ptr atom)
{
    return static_cast<IntAtom*>(atom.get());
}
#endif
