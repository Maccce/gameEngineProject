#pragma once

#include <memory>
#include <string>

namespace Engine
{
    enum class ErrorCode
    {
        Unknown,
        InvalidArgument,
        InvalidState,
        NotFound,
        IoError,
        UnsupportedVersion,
        InvalidData,
        InitializationFailed,
        ResourceCreationFailed,
        ExternalFailure
    };

    struct Error
    {
        ErrorCode code = ErrorCode::Unknown;
        std::string message;
        std::shared_ptr<const Error> cause;
    };
}
