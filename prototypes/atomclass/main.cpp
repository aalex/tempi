#include <iostream>
#include <tr1/memory>
#include <vector>
#include "atom.h"
#include "intatom.h"
#include "listatom.h"
#include "atomfactory.h"

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
 *  - boolean
 *  - float
 *  - string
 *  - null
 *  - list
 *  - dict
 *
 * We should add to this list at these types too:
 *
 *  - pointer
 *  - matrix
 *
 */

int main(int argc, char *argv[])
{
    std::vector<Atom::ptr> atoms;
    atoms.push_back(Atom::ptr(new IntAtom(2)));
    std::string json;
    atoms[0].get()->toJSON(json);
    std::cout << "to json: " << json << std::endl;;
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

    AtomFactory factory;
    factory.registerType<int, IntAtom>('i');
    std::cout << "Has 'i': " << factory.hasTypeName('i') << std::endl;
    //int i;
    std::cout << "Has int: " << factory.hasType(typeid(int)) << std::endl;
    Atom::ptr x = factory.create('i');
    INT_ATOM(x)->setInt(8);
    x->toJSON(json);
    std::cout << "create from type name: " << json << std::endl;
    //factory.create<int>(2)->toJSON(json);
    //std::cout << "create from type : " << json << std::endl;

    return 0;
}
