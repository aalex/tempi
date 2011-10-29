#include "tempi/table/table.h"

namespace tempi
{
namespace table
{

Table::Table()
{
    // pass
}

void Table::reset()
{
    messages_.clear();
}

unsigned int Table::getSize() const
{
    return messages_.size();
}

void Table::append(const Message &message)
{
    messages_.push_back(message);
}


bool Table::set(unsigned int index, const Message &message)
{
    if (index >= messages_.size())
        return false;
    messages_[index] = message;
    return true;
}

bool Table::get(unsigned int index, Message &message) const
{
    if (index >= messages_.size())
        return false;
    message = messages_[index];
    return true;
}

void Table::setSize(unsigned int size)
{
    messages_.resize(size);
}

} // end of namespace
} // end of namespace

