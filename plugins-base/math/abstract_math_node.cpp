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

#include "plugins-base/math/abstract_math_node.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi { 
namespace plugins_base {

const char * const AbstractMathNode::IN_INLET = "0";
const char * const AbstractMathNode::OUT_OUTLET = "0";

AbstractMathNode::AbstractMathNode() :
    Node()
{
    this->addInlet(IN_INLET, "Incoming float or list of floats.");
    this->addOutlet(OUT_OUTLET, "Resulting float or list of floats.");
}

void AbstractMathNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, IN_INLET))
    {
        Message result;
        for (Index i = 0; i < message.getSize(); i++)
        {
            AtomType atom_type;
            message.getAtomType(i, atom_type);
            Float value;
            bool ok = true;
            if (atom_type == FLOAT)
                value = message.getFloat(i);
            else if (atom_type == INT)
                value = (Float) message.getInt(i);
            else
                ok = false;
            if (ok)
            {
                result.appendFloat(this->calculate(value));
            }
            else
            {
                std::ostringstream os;
                os << "AbstractMathNode." << __FUNCTION__ << ": " <<
                "Bad value type " << atom_type << " at index " << i;
                Logger::log(ERROR, os);
            }
        } // for
        this->output("0", result);
    }
    else
    {
        this->processUnhandledMessage(inlet, message);
    }
}

} // end of namespace
} // end of namespace
