#include "tempi/table/table_player.h"
#include <iostream>

namespace tempi { namespace table {

TablePlayer::TablePlayer(Table *table) :
    table_(table),
    speed_(1)
{
    // pass
}

void TablePlayer::setTable(Table *table)
{
    table_ = table;
}

Table *TablePlayer::getTable() const
{
    return table_;
}

unsigned int TablePlayer::getIndex() const
{
    return index_;
}

bool TablePlayer::setIndex(unsigned int index)
{
    if (table_->getSize() == 0)
    {
        if (index == 0)
        {
            index_ = 0;
            return true;
        }
        else
        {
            index_ = 0;
            return false;
        }
    }
    else if (index >= table_->getSize())
    {
        index_ = table_->getSize();
        return false;
    }
    else
    {
        index_ = index;
        return true;
    }
}

bool TablePlayer::read(Message &message)
{
    if (! table_)
    {
        std::cout << "TablePlayer::" << __FUNCTION__ <<  "This table player has an invalid Table. Cannot read events." << std::endl;
        return false;
    }
    else if (table_->getSize() == 0)
    {
        //std::cout << "No point to read." << std::endl;
        return false;
    }
    else
    {
        table_->get(index_, message);
        index_ = (index_ + speed_) % table_->getSize();
        return true;
    }
}

void TablePlayer::setSpeed(unsigned int factor)
{
    speed_ = factor;
}

unsigned int TablePlayer::getSpeed() const
{
    return speed_;
}

} // end of namespace
} // end of namespace

