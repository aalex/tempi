// see http://stackoverflow.com/questions/2850213/dynamically-register-constructor-methods-in-an-abstractfactory-at-compile-time-us
#include <map>
#include <iostream>
#include <string>

class Node; // forward declaration

class NodeFactory
{
    public:
        class EntryBase
        {
            public:
                virtual const std::string &getId() const = 0;
                virtual Node *create() = 0;
        };
        /**
         * An entry in the registry.
         */
        template <class T>
        class Entry
        {
            public:
                virtual const std::string &getId() const
                {
                    return T::factory_key;
                }
                virtual Node *create()
                {
                    return new T();
                }
        };
        static std::string registerEntry(EntryBase *entry)
        {
            std::cout << "Register node type " << entry->getId() << std::endl;
            entries_[entry->getId()] = entry;
            return entry->getId();
        }
        static Node *create(const std::string &id)
        {
            return entries_[id]->create();
        }
    private:
        std::map<std::string, EntryBase *> entries_;
};

std::map<std::string, EntryBase *> NodeFactory::entries_;

class Node
{
    public:
        virtual ~Node()
        {
        }
        const std::string &getType() const
        {
            return type_;
        }
    private:
        std::string type_;
};

class 

int main(int argc, char **argv)
{
    Message *msg1;
    Message *msg2;

    msg1 = MessageFactory::Create(10);
    msg1->say();

    msg2 = MessageFactory::Create(11);
    msg2->say();

    delete msg1;
}

