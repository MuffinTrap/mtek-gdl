#pragma once

#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-assert.h>

/**
 * @file mgdl-dynamic_array.h
 * @ingroup core
 * @brief DynamicArray Class
 */

struct DynamicArray {
    void* data;
    sizetype capacity;
    sizetype count;
};
typedef struct DynamicArray DynamicArray;

// TODO General void* functions for assetmanager

#define DYNAMIC_ARRAY(TYPE) \
DynamicArray* DynamicArray_Create##TYPE(sizetype capacity); \
DynamicArray* DynamicArray_CreatePtr##TYPE(sizetype capacity); \
sizetype DynamicArray_Count##TYPE(DynamicArray* array); \
TYPE * DynamicArray_GetPtr##TYPE(DynamicArray* array, sizetype index); \
TYPE * DynamicArray_Get##TYPE(DynamicArray* array, sizetype index); \
sizetype DynamicArray_AddPtr##TYPE(DynamicArray* array, TYPE* item); \
sizetype DynamicArray_Add##TYPE(DynamicArray* array, TYPE item);

// ARRAY OF POINTERS TO TYPE and ARRAY OF TYPE
#define DYNAMIC_ARRAY_IMPL(TYPE) \
DynamicArray* DynamicArray_CreatePtr##TYPE(sizetype capacity) { \
    mgdl_assert_print(capacity > 0, "Cannot create empty DynamicArray for " #TYPE); \
    DynamicArray* arr = new DynamicArray(); \
    arr->data = (void*)malloc(sizeof(TYPE*) * capacity); \
    arr->capacity = capacity; \
    arr->count = 0; \
    return arr; \
} \
TYPE* DynamicArray_GetPtr##TYPE ( DynamicArray* array, sizetype index ) { \
    if (index < array->count) { \
        TYPE** v = (TYPE**)array->data; \
        return v[index]; \
    } else { \
        return nullptr; \
    } \
} \
sizetype DynamicArray_AddPtr##TYPE ( DynamicArray* array, TYPE* item ) {\
    if (array->count + 1 > array->capacity) { \
        array->data = (void*)realloc(array->data, sizeof(TYPE*) * array->capacity * 2); \
        array->capacity *= 2; \
    } \
    TYPE** v = (TYPE**)array->data; \
    v[array->count] = item; \
    array->count += 1; \
    return  array->count-1; \
} \
DynamicArray* DynamicArray_Create##TYPE(sizetype capacity) { \
    mgdl_assert_print(capacity > 0, "Cannot create empty DynamicArray for " #TYPE); \
    DynamicArray* arr = (DynamicArray*)malloc(sizeof(DynamicArray)); \
    sizetype membytes = sizeof(TYPE) * capacity;\
    mgdl_assert_print(membytes > 0, "Cannot malloc 0 bytes for dynamic array" #TYPE); \
    arr->data = (void*)malloc(membytes); \
    arr->capacity = capacity; \
    arr->count = 0; \
    return arr; \
} \
TYPE* DynamicArray_Get##TYPE ( DynamicArray* array, sizetype index ) { \
    if (index < array->count) { \
        TYPE* v = (TYPE*)array->data; \
        return &v[index]; \
    } else { \
        return nullptr; \
    } \
} \
sizetype DynamicArray_Add##TYPE ( DynamicArray* array, TYPE item ) {\
    if (array->count + 1 > array->capacity) { \
        array->data = (void*)realloc(array->data, sizeof(TYPE) * array->capacity * 2); \
        array->capacity *= 2; \
    } \
    TYPE* v = (TYPE*)array->data; \
    v[array->count] = item; \
    array->count += 1; \
    return  array->count-1; \
} \
sizetype DynamicArray_Count##TYPE(DynamicArray* array) { \
    return array->count; \
}
