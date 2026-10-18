#include "SharedLibrary.hpp"

#include <dlfcn.h>

#include <string>
#include <utility>

namespace App
{
    namespace
    {
        Engine::Error makeError(
            Engine::ErrorCode code,
            std::string message
        )
        {
            return Engine::Error{
                code,
                std::move(message),
                nullptr
            };
        }
    }

    std::string SharedLibrary::getPlatformLibraryName(
        const std::string& name
    )
    {
        return "lib" + name + ".so";
    }

    SharedLibrary::SharedLibrary(
        SharedLibrary&& other
    ) noexcept
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
            // Move assignment cannot return a cleanup error.
            cleanup();

            handle = other.handle;
            other.handle = nullptr;
        }

        return *this;
    }

    std::expected<void, Engine::Error> SharedLibrary::load(
        const std::string& path
    )
    {
        if (handle)
        {
            return std::unexpected(makeError(
                Engine::ErrorCode::InvalidState,
                "A shared library is already loaded."
            ));
        }

        // Clear any previous dynamic-loader error.
        (void)dlerror();

        void* loadedHandle = dlopen(path.c_str(), RTLD_NOW);

        if (!loadedHandle)
        {
            const char* error = dlerror();

            return std::unexpected(makeError(
                Engine::ErrorCode::IoError,
                "Failed to load shared library '" + path + "': " +
                    (error ? error : "Unknown dynamic loader error.")
            ));
        }

        handle = loadedHandle;
        return {};
    }

    std::expected<void*, Engine::Error> SharedLibrary::getSymbol(
        const char* name
    ) const
    {
        if (!name || name[0] == '\0')
        {
            return std::unexpected(makeError(
                Engine::ErrorCode::InvalidArgument,
                "Symbol name must not be null or empty."
            ));
        }

        if (!handle)
        {
            return std::unexpected(makeError(
                Engine::ErrorCode::InvalidState,
                "Cannot look up a symbol without a loaded library."
            ));
        }

        // POSIX requires checking dlerror(), rather than relying
        // only on the pointer returned by dlsym().
        (void)dlerror();

        void* symbol = dlsym(handle, name);

        if (const char* error = dlerror())
        {
            return std::unexpected(makeError(
                Engine::ErrorCode::NotFound,
                "Failed to find symbol '" + std::string(name) +
                    "': " + error
            ));
        }

        return symbol;
    }

    void SharedLibrary::cleanup() noexcept
    {
        if (!handle)
            return;

        void* handleToClose = handle;
        handle = nullptr;

        // Best-effort cleanup: this path cannot propagate errors.
        (void)dlclose(handleToClose);
    }

    std::expected<void, Engine::Error> SharedLibrary::unload()
    {
        if (!handle)
        {
            return {};
        }

        void* handleToClose = handle;
        handle = nullptr;

        if (dlclose(handleToClose) != 0)
        {
            const char* error = dlerror();

            return std::unexpected(makeError(
                Engine::ErrorCode::ExternalFailure,
                "Failed to unload shared library: " +
                    std::string(
                        error ? error : "Unknown dynamic loader error."
                    )
            ));
        }

        return {};
    }

    SharedLibrary::~SharedLibrary()
    {
        cleanup();
    }
}
