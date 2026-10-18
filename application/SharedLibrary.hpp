#pragma once

#include "core/Error.hpp"

#include <expected>
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

        std::expected<void, Engine::Error> load(
            const std::string& path
        );

        std::expected<void*, Engine::Error> getSymbol(
            const char* name
        ) const;

        std::expected<void, Engine::Error> unload();

    private:
        void cleanup() noexcept;

        void* handle = nullptr;
    };
}
