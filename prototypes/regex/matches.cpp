#include <stdlib.h>
#include <boost/regex.hpp>
#include <string>
#include <iostream>
#include <vector>

boost::regex pattern("([0-9]+)(\\-| |$)(.*)");

// process_ftp: 
// on success returns the ftp response code, and fills 
// msg with the ftp response message. 
int process_ftp(const char* text, std::string* msg)
{
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

int main(int argc, char * argv[])
{
    (void) argc;
    (void) argv;
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
        std::cout << "trying " << text << " with " << pattern << std::endl;
        std::cout << "retval = " << process_ftp(text.c_str(), & msg) << std::endl;
        std::cout << "result = " << msg << std::endl;
    }

    return 0;
}

