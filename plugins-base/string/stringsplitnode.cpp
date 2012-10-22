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
#include <string>
#include <vector>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include "plugins-base/string/stringsplitnode.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const StringSplitNode::STRING_INLET = "string";
const char * const StringSplitNode::TOKENS_OUTLET = "tokens";
const char * const StringSplitNode::SEPARATOR_ATTR = "regex";

static void split(std::vector<std::string> &result, const std::string &text, const std::string &regex);

void split(std::vector<std::string> &result, const std::string &text, const std::string &regex)
{
    boost::algorithm::split_regex(result, text, boost::regex(regex));
}

StringSplitNode::StringSplitNode() :
    Node()
{
    this->setShortDocumentation("Splits a string using a regular expression.");
    this->addAttribute(Attribute::ptr(new Attribute(SEPARATOR_ATTR,
        Message("s", " "), "Separator between elements. (A Perl-compatible regular expression.)")));
    this->addInlet(STRING_INLET, "String to split.");
    this->addOutlet(TOKENS_OUTLET, "Message containing the resulting string tokens.");
}

void StringSplitNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, STRING_INLET))
        return;
    if (! message.typesMatch("s"))
    {
        std::ostringstream os;
        os << "StringSplitNode." << __FUNCTION__ << ": Expected a single string as incoming message but got " <<
            message;
        Logger::log(WARNING, os);
        return;
    }
    Message ret;
    std::string incoming = message.getString(0);
    std::vector<std::string> result;
    split(result, incoming, this->getAttributeValue(SEPARATOR_ATTR).getString(0));
    std::vector<std::string>::const_iterator iter;
    for (iter = result.begin(); iter != result.end(); ++iter)
    {
        ret.appendString((*iter).c_str());
    }
    this->output(TOKENS_OUTLET, ret);
}

// bool StringSplitNode::onNodeAttributeChanged(const char *name, const Message &value)
// {
//     return true;
// }

} // end of namespace
} // end of namespace

