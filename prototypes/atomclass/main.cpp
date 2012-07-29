#include <iostream>
#include <tr1/memory>
#include <vector>
#include "atom.h"
#include "intatom.h"
#include "listatom.h"

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
