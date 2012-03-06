#include "abstractfactory.h"
#include "instancecreator.h"
#include <boost/any.hpp>
#include <iostream>
#include <vector>
#include <tr1/memory>

using namespace tempi;

// ----------------------------------------------------------------------
/**
 * The Atom class
 */
class Atom : public Instance
{
    public:
        const boost::any &getValue() const;
        bool setValue(const boost::any &value);
        // bool is(const char *name) const = 0;
    protected:
        Atom();
        Atom(const boost::any &value);
    private:
        boost::any value_;
};

Atom::Atom()
{
    value_ = boost::any();
}

Atom::Atom(const boost::any &value) :
    value_(value)
{
    // pass
}

const boost::any &Atom::getValue() const
{
    return value_;
}

bool Atom::setValue(const boost::any &value)
{
    value_ = value;
    return true;
}

class BadAtomIndex : public std::runtime_error
{
    public:
        BadAtomIndex(const char *message) :
            std::runtime_error(message)
        {}
};

// ----------------------------------------------------------------------
/**
 * An implementation of the Message class.
 */
class Message
{
    public:
        bool append(const Atom::ptr &atom);
        Instance::ptr get(unsigned int index)
            throw(BadAtomIndex);
        std::string getTypeTags() const;
        std::vector<Instance::ptr> getAtoms() const
        {
            return atoms_;
        }
    private:
        std::vector<Instance::ptr> atoms_;
};

// std::ostream &operator<<(std::ostream &os, const Message &message);

bool Message::append(const Atom::ptr &atom)
{
    atoms_.push_back(atom);
    return true;
}

Instance::ptr Message::get(unsigned int index)
    throw(BadAtomIndex)
{
    if (index >= atoms_.size())
    {
        std::ostringstream os;
        os << "Bad index for atom " << index;
        throw(BadAtomIndex(os.str().c_str()));
    }
    else
    {
        return atoms_[index];
    }
}

std::string Message::getTypeTags() const
{
    std::string ret;
    std::vector<Instance::ptr>::const_iterator iter;
    for (iter = atoms_.begin(); iter != atoms_.end(); ++iter)
        ret += (*iter)->getTypeName();
    return ret;
}

std::ostream &operator<<(std::ostream &os, const Message &message)
{
    os << "Message:" << std::endl;
    std::vector<Instance::ptr> atoms = message.getAtoms();
    std::vector<Instance::ptr>::const_iterator iter;
    for (iter = atoms.begin(); iter != atoms.end(); ++iter)
    {
        os << " * " << (*iter) << std::endl;
    }
    os << "----------------" << std::endl;
    return os;
}

// ----------------------------------------------------------------------
/**
 * This IntAtom class.
 */
class IntAtom : public Atom
{
    // pass
    public:
        static void append(Message &message, int value);
        static int get(const Message &message, unsigned int index)
            throw(BadElementTypeError, BadIndexError);
        static void set(Message &message, unsigned int index, int value)
            throw(BadElementTypeError, BadIndexError);
};

static void IntAtom::append(Message &message, int value)
{
    message.append(boost::any(

}

int get_int(const Instance &instance)
    throw(BadElementTypeError)
{
    Atom *atom = dynamic_cast<Atom>(instance.get());
    if (atom->getTypeName() == "i")
        return ())->;
    
}
void setInt(Message::ptr &instance, int value)
    throw(BadElementTypeError);

int getInt(const Instance::ptr &instance)
    throw(BadElementTypeError);

void setInt(Instance::ptr &instance, int value)
    throw(BadElementTypeError)
{
    if (instance.getTypeName() == "i")
        return <IntAtom*>(instance.get())->;

}

int getInt(const Instance::ptr &instance)
    throw(BadElementTypeError);
{
    
}

    Atom *atom = dynamic_cast<Atom>(instance.get());
    if (atom->getTypeName() == "i")
        return ())->;

// ----------------------------------------------------------------------
/**
 * This FloatAtom class.
 */
class FloatAtom : public Atom
{
    // pass
};

// ----------------------------------------------------------------------
int main(int argc, char *argv[])
{
    AbstractFactory factory;
    factory.registerType<IntAtom>("i");
    factory.registerType<FloatAtom>("f");

    Message m;
    {
        Instance::ptr atom = factory.create("i");
        Atom *pAtom = (Atom * ) atom.get();
        std::cout << "Created atom of type " << pAtom->getTypeName() << std::endl;
        m.append(atom);
    }
    {
        Instance::ptr atom = factory.create("f");
        Atom *pAtom = (Atom * ) atom.get();
        std::cout << "Created atom of type " << pAtom->getTypeName() << std::endl;
        m.append(atom);
    }

    std::cout << "Resulting type tag: " << m.getTypeTags() << std::endl;
    return 0;
}

