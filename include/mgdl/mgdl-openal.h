#pragma once

#include <mgdl/mgdl-types.h>

#ifdef __cplusplus
#include <limits>
#include <cstdio>
#include <type_traits>
#include <utility>
#else
#include <stdio.h>
#endif

// includes the OpenAL needed by platform
#ifdef GEKKO

/* Wii does not have OpenAL */

#else // PC platform

    #ifdef __APPLE__
        #include <OpenAL/al.h>
        #include <OpenAL/alc.h>
    #else
        // Needed on windows
        #ifdef __cplusplus
                #include <cstdint>
        #else
                #include <stdint.h>
        #endif
        #include <AL/al.h>
        #include <AL/alc.h>
    #endif

// Error checking functions

void check_al_errors(const char* filename, const sizetype line);

#ifdef __cplusplus

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
#endif

#endif // PC platform
