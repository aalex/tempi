/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot *
 * This file is part of Tempi.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of, either version 3 of the License, or
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

#include "tempi/loader.h"
#include "tempi/log.h"
//TODO #include "tempi/config.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h> // getenv
#include <sys/stat.h>
#include <vector>
#include <iostream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace tempi {

namespace { // anonymous namespace

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
    {
        std::ostringstream os;
        os << __FILE__ << ": " << __FUNCTION__ << ": TODO\n";
        Logger::log(WARNING, os);
        return ret; // FIXME
    }
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

Loader::Loader()
{
    // pass
}

Loader& Loader::getInstance()
{
    static Loader instance; // Guaranteed to be destroyed.
    return instance; // Instantiated on first use.
}

bool Loader::addPath(const char *path)
{
    std::string str = removeTrailingPathSep(path);
    if (hasPath(str.c_str()))
        return false;
    paths_.push_back(str);
    return true;
}

bool Loader::load(NodeFactory &factory, const char *name)
{
    if (getenv("PWD") != NULL)
    {
        std::ostringstream os;
        os << "pwd: " << getenv("PWD");
        Logger::log(DEBUG, os);
    }
    if (isLoaded(name))
    {
        std::ostringstream os;
        os << "Loader." << __FUNCTION__ << ": Plugin already loaded: " << name;
        Logger::log(INFO, os);
        return false;
    }
    std::vector<std::string>::const_iterator iter;
    for (iter = paths_.begin(); iter != paths_.end(); ++iter)
    {
        std::string prefix(*iter);
        std::string fileName;
        fileName = prefix + "/" + FILE_PREFIX + name + FILE_SUFFIX;
        {
            std::ostringstream os;
            os << __FUNCTION__ << ": Trying " << fileName;
            Logger::log(DEBUG, os);
        }
        if (fileExists(fileName.c_str()))
        {
            {
                std::ostringstream os;
                os << "dlopen()...\n";
                Logger::log(DEBUG, os);
            }
            void *dlobj = dlopen(fileName.c_str(), RTLD_NOW | RTLD_GLOBAL);
            if (! dlobj)
            {
                std::ostringstream os;
                os << "Loader." << __FUNCTION__ << ": Error loading " << fileName << ": " << dlerror();
                Logger::log(ERROR, os);
                return false;
            }
            std::string functionName;
            functionName = std::string(FUNC_PREFIX) + name + FUNC_SUFFIX;
            {
                std::ostringstream os;
                os << "Loader." << __FUNCTION__ << ": Retrieve symbol " <<  functionName.c_str();
                Logger::log(DEBUG, os);
            }
            SetupFunc functionPtr = (SetupFunc) dlsym(dlobj,  functionName.c_str());
            if (! functionPtr)
            {
                std::ostringstream os;
                os << "Loader." << __FUNCTION__ << ": Failed to retrieve symbol " <<  functionName.c_str() << "in " << fileName;
                Logger::log(ERROR, os);
                return false;
            }
            {
                std::ostringstream os;
                os << "Loader." << __FUNCTION__ << ": calling %s()..." <<  functionName;
                Logger::log(DEBUG, os);
            }
            (*functionPtr)(static_cast<void*>(&factory));
            loaded_.push_back(std::string(name));
            {
                std::ostringstream os;
                os << "Loader::" << __FUNCTION__ << ": Succesfully called " << functionName << " from " << fileName;
                Logger::log(DEBUG, os);
            }
            return true;
        }
        else
        {
            std::ostringstream os;
            os << "Loader::" << __FUNCTION__ << ": Could not find " << fileName;
            Logger::log(DEBUG, os);
        }
    }
    {
        std::ostringstream os;
        os << "Loader::" << __FUNCTION__ << ": Failed to load plugin " << name;
        Logger::log(ERROR, os);
    }
    return false;
}

bool Loader::isLoaded(const char *name)
{
    std::vector<std::string>::const_iterator iter;
    for (iter = loaded_.begin(); iter != loaded_.end(); ++iter)
        if ((*iter) == name)
            return true;
    return false;
}

bool Loader::hasPath(const char *path)
{
    std::vector<std::string>::const_iterator iter;
    for (iter = paths_.begin(); iter != paths_.end(); ++iter)
        if ((*iter) == path)
            return true;
    return false;
}

const char * const Loader::FILE_PREFIX = "libtempi-plugins-";
const char * const Loader::FILE_SUFFIX = ".so"; // TODO: if Linux
const char * const Loader::FUNC_PREFIX = "tempi_";
const char * const Loader::FUNC_SUFFIX = "_setup";
std::vector<std::string> Loader::loaded_;

} // end of namespace

