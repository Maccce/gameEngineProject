
#include "SharedLibrary.hpp"

#include <dlfcn.h>
#include <iostream>
#include <utility>

namespace App
{
    std::string SharedLibrary::getPlatformLibraryName(
        const std::string& name
    )
    {
        return "lib" + name + ".so";
    }

    SharedLibrary::SharedLibrary(SharedLibrary&& other) noexcept
        : handle(other.handle)
    {
        other.handle = nullptr;
    }

    SharedLibrary& SharedLibrary::operator=(
        SharedLibrary&& other
    ) noexcept
    {
        if (this != &other)
        {
            unload();

            handle = other.handle;
            other.handle = nullptr;
        }

        return *this;
    }

    bool SharedLibrary::load(const std::string& path)
    {
        if (handle)
        {
            std::cerr << "A shared library is already loaded.\n";
            return false;
        }

        handle = dlopen(path.c_str(), RTLD_NOW);

        if (!handle)
        {
            std::cerr << "Failed to load shared library: "
                      << dlerror() << '\n';
            return false;
        }

        return true;
    }

    void* SharedLibrary::getSymbol(const char* name) const
    {
        if (!handle)
        {
            return nullptr;
        }

        dlerror();
        void* symbol = dlsym(handle, name);

        if (const char* error = dlerror())
        {
            std::cerr << "Failed to find symbol '" << name
                      << "': " << error << '\n';
            return nullptr;
        }

        return symbol;
    }


    void SharedLibrary::unload()
    {
        if (handle)
        {
            if (dlclose(handle) != 0)
            {
                std::cerr << "Failed to unload shared library: "
                        << dlerror() << '\n';
            }

            handle = nullptr;
        }
    }


    SharedLibrary::~SharedLibrary()
    {
        unload();
    }
}
