#include <iostream>
#include "tempi/dynamicpatching.h"

using namespace tempi;

static const bool VERBOSE = false;

static bool splits_ok(const char *path, const std::vector<std::string> &expected, bool should_be_ok)
{
    std::vector<std::string> result;
    bool ok = dynamic::split_path(path, result);
    if (! ok && ! should_be_ok)
        return true;
    if (! ok && should_be_ok)
    {
        std::cout << "split_path returned false for path " << path << std::endl;
        return false;
    }
    else if (ok && ! should_be_ok)
    {
        std::cout << "split_path returned true for path " << path << std::endl;
        return false;
    }
    else
    {
        if (result.size() != expected.size())
        {
            std::cout << "length don't match expected for path " << path <<
                ". Expected " << expected.size() <<
                ". Got " << result.size() << std::endl;
       //     return false;
        }
        for (unsigned int i = 0; i < result.size(); i++)
        {
            if (VERBOSE)
                std::cout << "Element " << i << " in result: "  << result[i] << ", in expected: " << expected[i] << "." << std::endl;
            if (result[i] != expected[i])
            {
                std::cout << "Element " << result[i] << " doesn't match element " << expected[i] << "." << std::endl;
                return false;
            }
        }
        return true;
    }
}

static bool check_split_path()
{
    {
        std::string path = "/bar/egg/spam";
        std::vector<std::string> expected;
        expected.push_back("bar");
        expected.push_back("egg");
        expected.push_back("spam");
        if (! splits_ok(path.c_str(), expected, true))
            return false;
    }
    return true;
}



int main(int argc, char *argv[])
{
    if (! check_split_path())
        return 1;
    return 0;
}

