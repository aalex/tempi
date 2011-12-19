/**
 * Loads a hello.so
 * And calls void hello_setup()
 */
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <iostream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace tempi {

typedef void (*SetupFunc)(void);

namespace loader_utils {

std::string removeTrailingPathSep(const char *path)
{
    std::string ret(path);
    if (ret.size() == 0)
        return ret;
    if (ret.size() == 1)
        if (ret[0] == '/')
            return std::string();
        else
            return ret;
    if (ret[ret.size() - 1] == '/')
        std::cerr << __FILE__ << ": " << __FUNCTION__ << ": TODO\n";
        //return ret.clone(0, ret.size() - 2);
    return ret;
}

bool fileExists(const char *path)
{
    namespace fs = boost::filesystem;
    fs::path fullPath(path);
    return fs::exists(fullPath);
}

} // end of namespace

class Loader
{
    public:
        Loader() :
            verbose_(false)
        {}
        bool addPath(const char *path)
        {
            std::string str = loader_utils::removeTrailingPathSep(path);
            if (hasPath(str.c_str()))
                return false;
            paths_.push_back(str);
            return true;
        }
        bool load(const char *name)
        {
            if (verbose_)
                printf("isLoaded(%s)...\n", name);
            if (isLoaded(name))
            {
                std::cerr << "Already loaded: " << name << std::endl;
                return false;
            }
            std::vector<std::string>::const_iterator iter;
            for (iter = paths_.begin(); iter != paths_.end(); ++iter)
            {
                std::string prefix(*iter);
                std::string fileName;
                fileName = prefix + "/" + FILE_PREFIX + name + FILE_SUFFIX;
                if (verbose_)
                    std::cout << __FUNCTION__ << ": Trying " << fileName << std::endl;
                if (loader_utils::fileExists(fileName.c_str()))
                {
                    if (verbose_)
                        std::cout << "dlopen()...\n";
                    void *dlobj = dlopen(fileName.c_str(), RTLD_NOW | RTLD_GLOBAL);
                    if (! dlobj)
                    {
                        std::cerr << "Error loading " << fileName << ": " << dlerror() << std::endl;
                        return false;
                    }
                    std::string functionName;
                    functionName = std::string(FUNC_PREFIX) + name + FUNC_SUFFIX;
                    if (verbose_)
                        printf("Retrieve symbol %s...\n", functionName.c_str());
                    SetupFunc functionPtr = (SetupFunc) dlsym(dlobj,  functionName.c_str());
                    if (! functionPtr)
                    {
                        printf("failed\n");
                        return false;
                    }
                    if (verbose_)
                        printf("call %s()...\n", functionName.c_str());
                    (*functionPtr)();
                    loaded_.push_back(std::string(name));
                    if (verbose_)
                        printf("success!\n");
                    std::cout << "Loader::" << __FUNCTION__ << ": Succesfully loaded " << fileName << std::endl;
                    return true;
                }
                else
                {
                    std::cout << "Could not find " << fileName << std::endl;
                }
            }
            std::cout << "Failed to load " << name << std::endl;
            return false;
        }
        void setVerbose(bool verbose)
        {
            verbose_ = verbose;
        }
        bool isLoaded(const char *name)
        {
            std::vector<std::string>::const_iterator iter;
            for (iter = loaded_.begin(); iter != loaded_.end(); ++iter)
                if ((*iter) == name)
                    return true;
            return false;
        }
    private:
        bool verbose_;
        static std::vector<std::string> loaded_;
        std::vector<std::string> paths_;
        static const char * const FILE_PREFIX;
        static const char * const FILE_SUFFIX;
        static const char * const FUNC_PREFIX;
        static const char * const FUNC_SUFFIX;
        bool hasPath(const char *path)
        {
            std::vector<std::string>::const_iterator iter;
            for (iter = paths_.begin(); iter != paths_.end(); ++iter)
                if ((*iter) == path)
                    return true;
            return false;
        }
};

std::vector<std::string> Loader::loaded_;
const char * const Loader::FILE_PREFIX = "libtempi";
const char * const Loader::FILE_SUFFIX = ".so"; // TODO: if Linux
const char * const Loader::FUNC_PREFIX = "tempi_";
const char * const Loader::FUNC_SUFFIX = "_setup";

} // end of namespace

int main(int argc, char *argv[])
{
    std::cout << "create Loader" << std::endl;
    tempi::Loader loader;
    loader.setVerbose(true);
    loader.addPath(".");
    loader.addPath("/usr/local/lib/tempi-0.1");
    loader.addPath("/usr/lib/tempi-0.1");
    std::cout << "Loader::load(hello)" << std::endl;
    loader.load("hello");
    return 0;
}

