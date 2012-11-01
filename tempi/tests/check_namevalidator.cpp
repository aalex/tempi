#include <iostream>
#include "tempi/namevalidator.h"

using namespace tempi;
static const bool VERBOSE = true;

class Test
{
    public:
        Test(const char * pattern, const char * text, bool should_match) :
            pattern_(pattern), text_(text), should_match_(should_match)
        {}
        std::string pattern_;
        std::string text_;
        bool should_match_;
};

bool check_validator()
{
    bool ret = true;
    std::vector<Test> tests;
    tests.push_back(Test("", "123", true));
    tests.push_back(Test("", "hello", true));
    tests.push_back(Test("", "foo_bar", true));
    tests.push_back(Test("", "foo-bar", true));
    tests.push_back(Test("", "foo bar", false));
    tests.push_back(Test("", "foo.bar", false));

    std::vector<Test>::const_iterator iter;
    for (iter = tests.begin(); iter != tests.end(); ++iter)
    {
        NameValidator validator;
        if ((*iter).pattern_ != "")
        {
            validator.setPattern((*iter).pattern_.c_str());
        }
        bool ok = validator.matches((*iter).text_.c_str());
        bool should_match = (*iter).should_match_;

        if (ok && ! should_match)
        {
            std::cout << "Should not match but does: " << validator.getPattern() << " " << (*iter).text_ << std::endl;
            ret = false;
        }
        else if (! ok && should_match)
        {
            std::cout << "Should match but doesn't: " << validator.getPattern() << " " << (*iter).text_ << std::endl;
            ret = false;
        }
        else
        {
            std::cout << "OK: " << ok << " " << validator.getPattern() << " " << (*iter).text_ << std::endl;
        }
    }
    return ret;
}

int main(int argc, char *argv[])
{
    if (! check_validator())
        return 1;
    return 0;
}

