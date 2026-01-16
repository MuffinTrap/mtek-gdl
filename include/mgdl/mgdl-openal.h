#pragma once

/**
 * @file mgdl-openal.h
 * @ingroup audio
 * @brief Includes the OpenAL needed by platform
 */

# if defined(GEKKO) || defined(MGDL_PLATFORM_WINDOWS)
/* Wii does not have OpenAL */
/* Windows uses DirectSound */
#else

// Other platforms use OpenAL
#include <mgdl/mgdl-types.h>

#if defined(__cplusplus)
#   include <limits>
#   include <cstdio>
#   include <type_traits>
#   include <utility>
#else
#   include <stdio.h>
#endif

# if defined(__APPLE__)
#    include <OpenAL/al.h>
#    include <OpenAL/alc.h>
# elif defined(MGDL_PLATFORM_MSYS2)
#    if defined(__cplusplus)
#        include <cstdint>
#    else
#        include <stdint.h>
#    endif
#    include <AL/al.h>
#    include <AL/alc.h>
# elif defined(MGDL_PLATFORM_LINUX)
#    include <AL/al.h>
#    include <AL/alc.h>
# endif
// PC platform #endif at end of file

// Error checking functions

void check_al_errors(const char* filename, const sizetype line);

#if defined (__cplusplus)

template<typename alFunction, typename... Params>
auto alCallImpl(const char* filename, const std::uint_fast32_t line, alFunction function, Params... params)
->typename std::enable_if<std::is_same<void,decltype(function(params...))>::value,decltype(function(params...))>::type
{
    function(std::forward<Params>(params)...);
    check_al_errors(filename,line);
}

template<typename alFunction, typename... Params>
auto alCallImpl(const char* filename, const std::uint_fast32_t line, alFunction function, Params... params)
->typename std::enable_if<!std::is_same<void,decltype(function(params...))>::value,decltype(function(params...))>::type
{
    auto ret = function(std::forward<Params>(params)...);
    check_al_errors(filename,line);
    return ret;
}

// Wrapper to always error check Open AL calls
#define alCall(function, ...) alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)
#define alcCall(function, device, ...) alcCallImpl(__FILE__, __LINE__, function, device, __VA_ARGS__)

#else
// Wrapper to always error check Open AL calls
#define alCall(function, ...) function(__VA_ARGS__); check_al_errors(__FILE__, __LINE__);
#define alcCall(function, device, ...) return function(__VA_ARGS__);
#endif // __cplusplus

#endif // USES OPENAL
