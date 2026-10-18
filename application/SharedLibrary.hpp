
#pragma once

#include <string>

namespace App
{
    class SharedLibrary
    {
    public:
        SharedLibrary() = default;
        ~SharedLibrary();

        SharedLibrary(const SharedLibrary&) = delete;
        SharedLibrary& operator=(const SharedLibrary&) = delete;

        SharedLibrary(SharedLibrary&& other) noexcept;
        SharedLibrary& operator=(SharedLibrary&& other) noexcept;

        static std::string getPlatformLibraryName(
            const std::string& name
        );

        bool load(const std::string& path);
        void* getSymbol(const char* name) const;
        void unload();

    private:
        void* handle = nullptr;
    };
}
