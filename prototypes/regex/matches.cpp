#include <stdlib.h>
#include <boost/regex.hpp>
#include <string>
#include <iostream>
#include <vector>

// parse_first_int: 
// on success returns the ftp response code, and fills 
// msg with the ftp response message. 
int parse_first_int(const char* text, std::string* msg)
{
    boost::regex pattern("([0-9]+)(\\-| |$)(.*)");
    try
    {
        boost::cmatch what;
        if (boost::regex_match(text, what, pattern))
        {
            // what[0] contains the whole string 
            // what[1] contains the response code 
            // what[2] contains the separator character 
            // what[3] contains the text message. 
            if (msg)
            {
                msg->assign(what[3].first, what[3].second);
            }
            return std::atoi(what[1].first);
        }
        // failure did not match 
        if (msg)
        {
            msg->erase();
        }
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << __FUNCTION__ << ": " << e.what() << std::endl;
        return -1;
    }
    return -1;
}

class NameValidator
{
    public:
        NameValidator(const char *pattern);
        NameValidator();
        bool matches(const char * text);
        bool isOk() const;
        std::string getPattern() const;
        static const char * const DEFAULT_PATTERN;
    private:
        boost::regex pattern_;
        std::string text_pattern_;
        bool ok_;
};

const char * const NameValidator::DEFAULT_PATTERN = "^(\\w|-|_|\\.)+$";

NameValidator::NameValidator()
{
    this->pattern_ = DEFAULT_PATTERN;
    this->text_pattern_ = DEFAULT_PATTERN;
    this->ok_ = true;
}

NameValidator::NameValidator(const char *pattern) :
    ok_(false)
{
    try
    {
        this->pattern_ = std::string(pattern);
        this->text_pattern_ = pattern;
        this->ok_ = true;
    }
    catch (const boost::regex_error& e)
    {
        std::cerr << __FUNCTION__ << ": " << e.what() << std::endl;
    }
}

bool NameValidator::isOk() const
{
    return this->ok_;
}

std::string NameValidator::getPattern() const
{
    return this->text_pattern_;
}

bool NameValidator::matches(const char *text)
{
    try
    {
        boost::cmatch what;
        if (boost::regex_match(text, what, this->pattern_))
            return true;
        else
            return false;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "NameValidator::" << __FUNCTION__ << ": " << e.what() << std::endl;
        return false;
    }
    return false;
}

bool check_int()
{
    std::vector<std::string> strings;
    strings.push_back(std::string("12345678"));
    strings.push_back(std::string("123abc"));
    strings.push_back(std::string("123-"));
    strings.push_back(std::string("123 "));
    strings.push_back(std::string("toto"));
    strings.push_back(std::string("toto-123"));
    strings.push_back(std::string("jesus"));
    
    std::vector<std::string>::const_iterator iter;
    for (iter = strings.begin(); iter != strings.end(); ++iter)
    {
        std::string msg;
        std::string text = (*iter);
        std::cout << "trying " << text << std::endl;
        std::cout << "retval = " << parse_first_int(text.c_str(), & msg) << std::endl;
        std::cout << "result = " << msg << std::endl;
    }
    return true;
}

bool check_validator()
{
    std::vector<std::string> strings;
    strings.push_back(std::string("12345678"));
    strings.push_back(std::string("123abc"));
    strings.push_back(std::string("123-"));
    strings.push_back(std::string("123 "));
    strings.push_back(std::string("toto"));
    strings.push_back(std::string("toto-123"));
    strings.push_back(std::string("jesus"));

    NameValidator a("([0-9]|[a-zA-Z]|-|_)+$");
    NameValidator b;
    
    std::vector<std::string>::const_iterator iter;
    for (iter = strings.begin(); iter != strings.end(); ++iter)
    {
        std::cout << "Check " << (*iter) << " against " << a.getPattern() << ": " << a.matches((*iter).c_str()) << std::endl;
        std::cout << "Check " << (*iter) << " against " << b.getPattern() << ": " << b.matches((*iter).c_str()) << std::endl;
    }
    return true;
}

int main(int argc, char * argv[])
{
    (void) argc;
    (void) argv;
    if (! check_int())
        return 1;
    if (! check_validator())
        return 1;
    return 0;
}

