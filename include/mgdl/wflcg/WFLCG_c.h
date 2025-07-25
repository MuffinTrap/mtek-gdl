#ifndef WFLCG_C_INCLUDE_GUARD
#define WFLCG_C_INCLUDE_GUARD

#include <stdint.h>

#define WFLCG_C_VERSION 0x010001
#define WFLCG_C_VERSION_STRING "1.0.1"
#define WFLCG_C_COPYRIGHT_STRING "WFLCG v" WFLCG_C_VERSION_STRING " (C)2019 Juha Nieminen"

#define WFLCG_C_BUFFER_SIZE 16

typedef struct
{
    uint32_t buffer[WFLCG_C_BUFFER_SIZE];
    unsigned index;
} WFLCG_c;

inline void WFLCG_c_init_1_seed(WFLCG_c* obj, uint32_t seed)
{
    seed = seed * UINT32_C(2364742333) + UINT32_C(14567);
    for(unsigned i = 0; i < WFLCG_C_BUFFER_SIZE; ++i)
    {
        seed = seed * UINT32_C(2364742333) + UINT32_C(14567);
        obj->buffer[i] = seed;
    }
    obj->index = 0;
}

inline void WFLCG_c_init_2_seeds(WFLCG_c* obj, uint32_t seed1, uint32_t seed2)
{
    seed1 = seed1 * UINT32_C(2364742333) + UINT32_C(14567);
    seed2 = seed2 * UINT32_C(4112992229) + UINT32_C(12345);
    for(unsigned i = 0; i < WFLCG_C_BUFFER_SIZE; i += 2)
    {
        seed1 = seed1 * UINT32_C(2364742333) + UINT32_C(14567);
        seed2 = seed2 * UINT32_C(4112992229) + UINT32_C(12345);
        obj->buffer[i] = seed1;
        obj->buffer[i+1] = seed2;
    }
    obj->index = 0;
}

inline void WFLCG_c_init_default(WFLCG_c* obj)
{
    WFLCG_c_init_1_seed(obj, 0);
}

inline void WFLCG_c_refill_buffer(WFLCG_c* obj)
{
    static const uint32_t multipliers[WFLCG_C_BUFFER_SIZE] =
    {
        UINT32_C(3363461597), UINT32_C(3169304909), UINT32_C(2169304933), UINT32_C(2958304901),
        UINT32_C(2738319061), UINT32_C(2738319613), UINT32_C(3238311437), UINT32_C(1238311381),
        UINT32_C(1964742293), UINT32_C(1964743093), UINT32_C(2364742333), UINT32_C(2312912477),
        UINT32_C(2312913061), UINT32_C(1312912501), UINT32_C(2812992317), UINT32_C(4112992229)
    };
    static const uint32_t increments[WFLCG_C_BUFFER_SIZE] =
    {
        UINT32_C(8346591), UINT32_C(18134761), UINT32_C(12345), UINT32_C(234567),
        UINT32_C(14567), UINT32_C(12345), UINT32_C(123123), UINT32_C(11223345),
        UINT32_C(123131), UINT32_C(83851), UINT32_C(14567), UINT32_C(134567),
        UINT32_C(34567), UINT32_C(32145), UINT32_C(123093), UINT32_C(12345)
    };

    for(unsigned i = 0; i < WFLCG_C_BUFFER_SIZE; ++i)
        obj->buffer[i] = obj->buffer[i] * multipliers[i] + increments[i];

    obj->index = 0;
}

inline uint32_t WFLCG_c_get_value(WFLCG_c* obj)
{
    if(obj->index == WFLCG_C_BUFFER_SIZE) WFLCG_c_refill_buffer(obj);
    const uint32_t result = obj->buffer[obj->index] ^ (obj->buffer[obj->index] >> 24);
    ++obj->index;
    return result;
}

inline float WFLCG_c_get_float(WFLCG_c* obj)
{
    if(obj->index == WFLCG_C_BUFFER_SIZE) WFLCG_c_refill_buffer(obj);
    union { float fValue; uint32_t uValue; } conv;
    conv.uValue = UINT32_C(0x3F800000) | (obj->buffer[obj->index++] >> 9);
    return conv.fValue;
}

inline double WFLCG_c_get_double(WFLCG_c* obj)
{
    union { double dValue; uint64_t uValue; } conv;
    conv.uValue = (UINT64_C(0x3FF0000000000000) | (((uint64_t)WFLCG_c_get_value(obj)) << 20));
    return conv.dValue;
}

inline double WFLCG_c_get_double2(WFLCG_c* obj)
{
    if(obj->index == WFLCG_C_BUFFER_SIZE) WFLCG_c_refill_buffer(obj);
    union { double dValue; uint64_t uValue; } conv;
    uint32_t value1 = obj->buffer[obj->index], value2 = obj->buffer[obj->index+1];
    conv.uValue = (UINT64_C(0x3FF0000000000000) |
                   ((((uint64_t)value1) << 20) ^
                    (((uint64_t)value2) >> 4)));
    obj->index += 2;
    return conv.dValue;
}

inline float WFLCG_c_buffer_element_float(WFLCG_c* obj, unsigned index)
{
    union { float fValue; uint32_t uValue; } conv;
    conv.uValue = UINT32_C(0x3F800000) | (obj->buffer[index] >> 9);
    return conv.fValue;
}

inline double WFLCG_c_buffer_element_double(WFLCG_c* obj, unsigned index)
{
    union { double dValue; uint64_t uValue; } conv;
    uint32_t value = obj->buffer[index];
    value ^= value >> 24;
    conv.uValue = (UINT64_C(0x3FF0000000000000) | (((uint64_t)value) << 20));
    return conv.dValue;
}

inline double WFLCG_c_buffer_element_double2(WFLCG_c* obj, unsigned index)
{
    union { double dValue; uint64_t uValue; } conv;
    uint32_t value1 = obj->buffer[index], value2 = obj->buffer[index+1];
    conv.uValue = (UINT64_C(0x3FF0000000000000) |
                   ((((uint64_t)value1) << 20) ^ (((uint64_t)value2) >> 4)));
    return conv.dValue;
}
#endif
