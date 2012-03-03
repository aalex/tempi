#include "tempi/nodefactory.h"
#include <boost/any.hpp>
#include <iostream>
#include <tr1/memory>

class Atom : public tempi::Node
{
    public:
        typedef std::tr1::shared_ptr<Atom> ptr;
        Atom();
        Atom(const boost::any &value);
        virtual char getTypeTag() const = 0;
        const boost::any &getValue() const;
        bool setValue(const boost::any &value);
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

class AtomFactory : public tempi::NodeFactory
{
    public:
        template <class T>
        bool registerAtomType()
        {
            using namespace tempi;
            AbstractNodeType *tmp = (AbstractNodeType*) new NodeType<T>();
            Atom *atomType = dynamic_cast<Atom*>(tmp);
            AbstractNodeType::ptr ret(tmp);
            std::string typeName;
            typeName.push_back(atomType->getTypeTag());
            if (! this->registerType(typeName.c_str(), ret))
            {
                std::cerr << "Could not register type " << atomType->getTypeTag() << std::endl;
                return false;
            }
            return true;
        }
        // Atom::ptr createAtom(const char typeTag)
        //     throw(BadNodeTypeException)
        // {
        //     Atom::ptr ret;
        //     ret.reset(create(std::string(typeTag).c_str()).get());
        //     return ret;
        // }
};

class Message
{
    public:
        bool append(const Atom::ptr &atom);
        bool get(unsigned int index, Atom::ptr &result);
        std::string getTypeTags() const;
    private:
        std::vector<Atom::ptr> atoms_;
};

bool Message::append(const Atom::ptr &atom)
{
    atoms_.push_back(Atom::ptr(atom.get()));
    return true;
}

bool Message::get(unsigned int index, Atom::ptr &result)
{
    if (index >= atoms_.size())
        return false;
    else
    {
        result.reset(atoms_[index].get());
        return true;
    }
}

std::string Message::getTypeTags() const
{
    std::string ret;
    std::vector<Atom::ptr>::const_iterator iter;
    for (iter = atoms_.begin(); iter != atoms_.end(); ++iter)
        ret.push_back((*iter)->getTypeTag());
    return ret;
}

class IntAtom : public Atom
{
    public:
        virtual char getTypeTag() const;
};

char IntAtom::getTypeTag() const
{
    return 'i';
}

class FloatAtom : public Atom
{
    public:
        virtual char getTypeTag() const;
};

char FloatAtom::getTypeTag() const
{
    return 'f';
}

int main(int argc, char *argv[])
{
    AtomFactory factory;
    factory.registerAtomType<IntAtom>();
    factory.registerAtomType<FloatAtom>();

    tempi::Node::ptr atom = factory.create("i");
    Atom *pAtom = (Atom * ) atom.get();
    std::cout << pAtom->getTypeTag() << ": " << std::endl;
    return 0;
}

