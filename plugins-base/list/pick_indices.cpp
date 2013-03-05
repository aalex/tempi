/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 * Copyright (C) 2012 Emmanuel Durand
 *
 * This file is part of Tempi-plugins-base.
 *
 * This program is free software; you can redistither version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Tempi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include "plugins-base/list/pick_indices.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const PickIndicesNode::IN_INLET = "in";
const char * const PickIndicesNode::OUT_OUTLET = "out";
const char * const PickIndicesNode::INDICES_ATTR = "indices";

PickIndicesNode::PickIndicesNode() :
    Node()
{
    this->addAttribute(Attribute::ptr(new Attribute(INDICES_ATTR, Message("i", 0), "List of index numbers to pick in the incoming messages.", false))); // not type strict
    this->addInlet(IN_INLET, "Incoming messages go here.");
    this->addOutlet(OUT_OUTLET, "Result are output there.");
    this->setShortDocumentation("Picks values in incoming messages, according the specified index numbers.");
    this->setLongDocumentation("Note that it uses the modulo of these index numbers on the size of the incoming messages. That makes it possible to use negative value. For example, index -1 will give you the last value.");
}

static bool messageToListOfInts(const Message &message, std::vector<int> &result)
{
    for (unsigned int i = 0; i < message.getSize(); i++)
    {
        bool ok = false;
        int index = 0;
        if (message.indexMatchesType(i, INT))
        {
            index = message.getInt(i);
            ok = true;
        }
        else if (message.indexMatchesType(i, FLOAT))
        {

            index = (int) message.getFloat(i);
            ok = true;
        }
        else
            ok = false;
        if (ok)
        {
            result.push_back(index);
        }
        else
            return false;
    }
    return true;
}

static bool pickIndices(const Message &source, const std::vector<int> &indices, Message &result)
{
    std::vector<int>::const_iterator iter;
    for (iter = indices.begin(); iter != indices.end(); ++iter)
    {
        int index = (*iter);
        index = index % source.getSize();
        // XXX in all cases we use the modulo
        // if (index < 0)
        // {
        //     index = index % source.getSize();
        // }
        AtomType atom_type;
        source.getAtomType(index, atom_type);

        switch (atom_type)
        {
            case BOOLEAN:
                result.appendBoolean(source.getBoolean(index));
                break;
            case FLOAT:
                result.appendFloat(source.getFloat(index));
                break;
            case INT:
                result.appendInt(source.getInt(index));
                break;
            case STRING:
                result.appendString(source.getString(index).c_str());
                break;
            case POINTER:
                result.appendPointer(source.getPointer(index));
                break;
            case BLOB:
                result.appendBlob(source.getBlob(index));
                break;
            default:
            {
                std::ostringstream os;
                os << __FUNCTION__ << ": Unsupported value type \"" << atom_type << "\" at index " << index;
                Logger::log(ERROR, os);
                break;
            }
        } // end of switch/case
    }
    return true;
}

void PickIndicesNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, IN_INLET))
        return;

    std::vector<int> indices;
    messageToListOfInts(this->getAttributeValue(INDICES_ATTR), indices);

    // if (Logger::isEnabledFor(DEBUG))
    // {
    //     std::ostringstream os;
    //     os << "PickIndicesNode." << __FUNCTION__ << ": indices " << indices;
    //     Logger::log(DEBUG, os);
    // }

    Message result;
    if (pickIndices(message, indices, result))
        this->output(OUT_OUTLET, result);
    else
    {
        // std::ostringstream os;
        // os << "PickIndicesNode." << __FUNCTION__ << ": failed to pick " << indices;
        // Logger::log(ERROR, os);
    }
}

bool PickIndicesNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (utils::stringsMatch(name, INDICES_ATTR))
    {
        std::vector<int> indices;
        if (messageToListOfInts(value, indices))
            return true; // true means it's OK to change that attribute
        else
            return false;
    }
    else
        return true;
}

} // end of namespace
} // end of namespace

