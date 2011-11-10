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

typedef void (*SetupFunc)(void);

class Loader
{
    public:
        Loader()
        {}
        bool load(const char *name)
        {
            printf("isLoaded(%s)...\n", name);
            if (isLoaded(name))
            {
                std::cerr << "Already loaded: " << name << std::endl;
                return false;
            }
            printf("yes!\n");
            std::string fileName(name);
            fileName += ".so"; // FIXME
            fileName = std::string("./") + fileName; // FIXME
            printf("fileName = %s\n", fileName.c_str());
            // TODO: check if fileName exists
            printf("dlopen()...\n");
            void *dlobj = dlopen(fileName.c_str(), RTLD_NOW | RTLD_GLOBAL);
            if (! dlobj)
            {
                std::cerr << "Error loading " << fileName << ": " << dlerror() << std::endl;
                return false;
            }
            printf("ok\n");
            std::string functionName(name); // FIXME
            functionName += "_setup";
            printf("Retrieve symbol %s...\n", functionName.c_str());
            SetupFunc functionPtr = (SetupFunc) dlsym(dlobj,  functionName.c_str());
            if (! functionPtr)
            {
                printf("failed\n");
                return false;
            }
            printf("ok\n");
            printf("call %s()...\n", functionName.c_str());
            (*functionPtr)();
            printf("ok\n");
            loaded_.push_back(std::string(name));
            printf("success!\n");
            return true;
        }
        bool isLoaded(const char *name)
        {
            std::vector<std::string>::const_iterator iter;
            for (iter = loaded_.begin(); iter != loaded_.end(); ++iter)
                if ((*iter).c_str() == name)
                    return true;
            return false;
        }
    private:
        static std::vector<std::string> loaded_;
};

std::vector<std::string> Loader::loaded_;

int main(int argc, char *argv[])
{
    std::cout << "create Loader" << std::endl;
    Loader loader;
    std::cout << "Loader::load(hello)" << std::endl;
    loader.load("hello");
    return 0;
}

