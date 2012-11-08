#include <v8.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <stdexcept>

class JavaScriptError : public std::runtime_error
{
    public:
        JavaScriptError(const char *message) :
            std::runtime_error(message)
        {}
};

double calculate(const char *formula, const std::vector<double> &args)
    throw(JavaScriptError)
{
    v8::HandleScope scope;
    v8::Persistent<v8::Context> context = v8::Context::New();
    v8::Context::Scope context_scope(context);

    std::ostringstream code;
    for (unsigned int i = 0; i < args.size(); i++)
    {
        code << "var f" << i << " = " << args[i] << ";" << std::endl;;
    }
    code << formula << ";" << std::endl;

    std::cout << "-----------------\n";
    std::cout << "CODE: " << std::endl;
    std::cout << code.str().c_str();
    std::cout << "-----------------\n";

    v8::Handle<v8::String> source = v8::String::New(code.str().c_str());
    v8::Handle<v8::Script> script = v8::Script::Compile(source);
    v8::TryCatch try_catch;
    // // handle exception:
    // {
    //     //v8::Handle<v8::Message> message = try_catch.Message();
    //     std::ostringstream os;
    //     os << __FUNCTION__ << ": Could not compile script \"" << formula; //  << "\": " << (* message.Get());
    //     throw (JavaScriptError(os.str().c_str()));
    // }
    if (script.IsEmpty())
    {
        std::ostringstream os;
        os << __FUNCTION__ << ": Could not compile script \"" << formula << "\"";
        throw (JavaScriptError(os.str().c_str()));
    }
    v8::Handle<v8::Value> result = script->Run();
    if (! result->IsNumber())
    {
        std::ostringstream os;
        os << __FUNCTION__ << ": Result is not a number: \"" << formula << "\"";
        throw (JavaScriptError(os.str().c_str()));
    }

    v8::Number *number = v8::Number::Cast(*result);

    double ret = number->Value();
    std::cout << ret << std::endl;
    return ret;
}

bool check_expr()
{
    std::vector<double> args;
    args.push_back(2.0);
    args.push_back(10.0);
    args.push_back(2.0);
    try
    {
        double result = calculate("Math.pow(f0, f1)", args);
        std::cout << result << std::endl;
    }
    catch (const JavaScriptError &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char ** argv)
{
    (void) argc;
    (void) argv;

    if (! check_expr())
        return 1;

    return 0;
}
