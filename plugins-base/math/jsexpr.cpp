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

#include "tempi/config.h" // include before check for HAVE_V8

#ifdef HAVE_V8

#include <iostream>
#include <v8.h>
#include <vector>
#include <sstream>
#include <stdexcept>
#include "plugins-base/math/jsexpr.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const JsExprNode::INPUT_INLET = "input";
const char * const JsExprNode::OUTPUT_OUTLET = "output";
const char * const JsExprNode::SCRIPT_ATTR = "script";

JsExprNode::JsExprNode() :
    Node()
{
    addAttribute(Attribute::ptr(new Attribute(SCRIPT_ATTR, Message("s", "args"), "Javascript expression to run and get its return value as a result.")));
    Logger::log(DEBUG, "[expr]");
    addInlet(INPUT_INLET, "Incoming messages go here. It will be accessible via the args variable in the javascript expression");
    addOutlet(OUTPUT_OUTLET, "Outputs the return value of the javascript expression");
    setShortDocumentation("Runs a javascript expression and outputs the result.");
}

void JsExprNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, INPUT_INLET))
        return;
    std::string script = this->getAttributeValue(SCRIPT_ATTR).getString(0);

    Message result;
    if (runJavaScript(script.c_str(), message, result))
    {
        if (Logger::isEnabledFor(INFO))
        {
            std::ostringstream os;
            os << "[expr]: " << script << " " << " == " <<  result;
            Logger::log(INFO, os);
        }
        output(OUTPUT_OUTLET, result);
    }
}

bool JsExprNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "[expr] " << __FUNCTION__ << ": name=\"" << name << "\" value=" << value;
        Logger::log(DEBUG, os);
    }
    return true; // true means it's OK to change that attribute
}

/**
 * build a string like [2, 3.14159, "hello"]
 */
bool messageToJavascript(const Message &message, std::string &result)
{
    bool ok = true;
    std::ostringstream os;
    os << "[";
    unsigned int message_length = message.getSize();
    for (unsigned int i = 0; i < message_length; i++)
    {
        tempi::AtomType type;
        message.getAtomType(i, type);
        switch (type)
        {
            case tempi::BOOLEAN:
                if (message.getBoolean(i))
                    os << "true";
                else
                    os << "false";
                break;
            case tempi::FLOAT:
                os << message.getFloat(i);
                break;
            case tempi::INT:
                os << message.getInt(i);
                break;
            case tempi::STRING:
                os << "\"" << message.getString(i) << "\"";
                break;
            case tempi::POINTER:
            default:
                ok = false;
                {
                    std::ostringstream os2;
                    os2 << __FUNCTION__ << ": Unsupported tempi type tag.\n";
                    Logger::log(WARNING, os2);
                }
                break;
        }
        if (i != message.getSize() - 1)
            os << ", ";
    }
    os << "]";
    result = os.str();
    return ok;
}

bool javaScriptToMessage(v8::Local<v8::Value> value, Message &result)
{
    bool success = true;
    if (value->IsNumber())
    {
        double number = value->ToNumber()->Value();
        result.appendFloat(number);
    }
    else if (value->IsString())
    {
        v8::String::AsciiValue tmp(value->ToString());
        result.appendString(*tmp);
    }
    else if (value->IsArray())
    {
        // disabled for now:
        {
            std::ostringstream os;
            os << __FUNCTION__ << ": Unsupported type: array.";
            Logger::log(WARNING, os);
        }
        return false;
        // the code below is not executed, but it's a work in progress

        v8::Array *array = v8::Array::Cast(*value);
        unsigned int length = array->Length();
        //std::cout << __FUNCTION__ << ": array length : " << length << std::endl;

        for (unsigned int i = 0; i < array->Length(); i++)
        {
            v8::Local<v8::Object> element = array->CloneElementAt(i);
            if (*element == 0)
            {
                std::ostringstream os;
                os << __FUNCTION__ << ": array element is NULL";
                Logger::log(ERROR, os);
                return false;
            }
            //std::cout << __FUNCTION__ << " element: " << *element << std::endl;
            if (element->IsNumber())
            {
                double number = v8::NumberObject::Cast(*element)->NumberValue();
                result.appendFloat(number);
            }
            else if (element->IsString())
            {
                v8::String::AsciiValue tmp(element->ToString());
                result.appendString(*tmp);
            }
            else
            {
                success = false;
                {
                    std::ostringstream os;
                    os << __FUNCTION__ << ": Unsupported type.";
                    Logger::log(WARNING, os);
                }
            }
        }
    }
    else
    {
        success = false;
        {
            std::ostringstream os;
            os << __FUNCTION__ << ": Unsupported type.";
            Logger::log(WARNING, os);
        }
    }
    return success;
}

bool runJavaScript(const char *source, const Message &args, Message &result)
{
    v8::HandleScope scope;
    v8::Persistent<v8::Context> context = v8::Context::New();
    v8::Context::Scope context_scope(context);

    std::ostringstream code;

    std::string args_text;
    bool ok = messageToJavascript(args, args_text);
    (void) ok;
    code << "var args = " << args_text << ";" << std::endl;
    code << "var func = function(args) { ";
    code << source;
    code << " }" << std::endl;
    code << "func(args);";

    Logger::log(INFO, code);

    v8::Handle<v8::String> source_v8_txt = v8::String::New(code.str().c_str());
    v8::Handle<v8::Script> script = v8::Script::Compile(source_v8_txt);
    v8::TryCatch try_catch;
    // // handle exception:
    // {
    //     //v8::Handle<v8::Message> message = try_catch.Message();
    //     std::ostringstream os;
    //     os << __FUNCTION__ << ": Could not compile script \"" << script; //  << "\": " << (* message.Get());
    //     throw (JavaScriptError(os.str().c_str()));
    // }
    if (script.IsEmpty())
    {
        std::ostringstream os;
        os << __FUNCTION__ << ": Could not compile script \"" << code.str() << "\"";
        Logger::log(ERROR, os);
        return false;
    }
    v8::Local<v8::Value> v8_handle = script->Run();

    //v8::Value *v8_value = (*v8_handle);
    bool ret = javaScriptToMessage(v8_handle, result);
    
    return ret && ok;
}

} // end of namespace
} // end of namespace

#endif // HAVE_V8
