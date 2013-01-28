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

#include "tempi/config.h" // include before check for HAVE_LUA
#ifdef HAVE_LUA

#include "plugins-base/math/luaexpr.h"
extern "C" {
#include "plugins-base/math/ae.h"
} // extern "C"
#include <iostream>
#include <sstream>
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const LuaExprNode::INPUT_INLET = "in";
const char * const LuaExprNode::OUTPUT_OUTLET = "out";
const char * const LuaExprNode::SCRIPT_ATTR = "expression";

LuaExprNode::LuaExprNode() :
    Node()
{
    this->addAttribute(Attribute::ptr(new Attribute(SCRIPT_ATTR, Message("s", "f0"), "Lua expression to run and get its return value as a result.")));
    //Logger::log(DEBUG, "[expr]");
    this->addInlet(INPUT_INLET, "Incoming messages go here. It will be accessible via the f0 variable in the Lua expression", "", "f");
    this->addOutlet(OUTPUT_OUTLET, "Outputs the return value of the Lua expression");
    this->setShortDocumentation("Runs a Lua expression and outputs the result.");
    this->setLongDocumentation(
        "Examples\n"
        "~~~~~~~~\n"
        "f0\n"
        "f0 + 2\n"
        "sin(f0)\n"
        "sqrt(f0 + 1) * 2\n"
        );
}

void LuaExprNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, INPUT_INLET))
        return;
    std::string script = this->getAttributeValue(SCRIPT_ATTR).getString(0);

    Message result;
    ae_open();
    ae_set("f0", (double) message.getFloat(0));
    if (Logger::isEnabledFor(INFO))
    {
        std::ostringstream os;
        os << "[expr]: " << script << " " << " == " <<  result;
        Logger::log(INFO, os);
    }
    result.appendFloat( (float) ae_eval(script.c_str()));
    if (ae_error() != NULL)
    {
        std::ostringstream os;
        os << "[expr]: " << ae_error() << " ::: " << script;
    }
    else
        output(OUTPUT_OUTLET, result);
}

bool LuaExprNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "[expr] " << __FUNCTION__ << ": name=\"" << name << "\" value=" << value;
        Logger::log(DEBUG, os);
    }
    return true; // true means it's OK to change that attribute
}

} // end of namespace
} // end of namespace

#endif // HAVE_LUA

