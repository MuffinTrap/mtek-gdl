#pragma once

#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-assert.h>

/**
 * @file mgdl-dynamic_array.h
 * @ingroup core
 * @brief DynamicArray types
 */

struct DynamicArray {
    void* data;
    sizetype capacity;
    sizetype count;
};
typedef struct DynamicArray DynamicArray;
typedef DynamicArray ObjectArray;
typedef DynamicArray PointerArray;

sizetype ObjectArray_Count(ObjectArray* array);
sizetype PointerArray_Count(PointerArray* array);

#define POINTER_ARRAY_DECLARE(TYPE) \
PointerArray* PointerArray_Create_##TYPE(sizetype capacity); \
TYPE * PointerArray_Get_##TYPE(PointerArray* array, sizetype index); \
sizetype PointerArray_Add_##TYPE(PointerArray* array, TYPE* item); \

#define OBJECT_ARRAY_DECLARE(TYPE) \
ObjectArray* ObjectArray_Create_##TYPE(sizetype capacity); \
TYPE * ObjectArray_Get_##TYPE(ObjectArray* array, sizetype index); \
sizetype ObjectArray_Add_##TYPE(ObjectArray* array, TYPE item);

// ARRAY OF POINTERS TO TYPE and ARRAY OF TYPE
#define POINTER_ARRAY_IMPLEMENT(TYPE) \
PointerArray* PointerArray_Create_##TYPE(sizetype capacity) { \
    mgdl_assert_print(capacity > 0, "Cannot create empty PointerArray for " #TYPE); \
    PointerArray* arr = (PointerArray*)malloc(sizeof(DynamicArray)); \
    arr->data = (void*)malloc(sizeof(TYPE*) * capacity); \
    arr->capacity = capacity; \
    arr->count = 0; \
    return arr; \
} \
TYPE* PointerArray_Get_##TYPE ( PointerArray* array, sizetype index ) { \
    if (index < array->count) { \
        TYPE** v = (TYPE**)array->data; \
        return v[index]; \
    } else { \
        return nullptr; \
    } \
} \
sizetype PointerArray_Add_##TYPE ( PointerArray* array, TYPE* item ) {\
    if (array->count + 1 > array->capacity) { \
        array->data = (void*)realloc(array->data, sizeof(TYPE*) * array->capacity * 2); \
        array->capacity *= 2; \
    } \
    TYPE** v = (TYPE**)array->data; \
    v[array->count] = item; \
    array->count += 1; \
    return  array->count-1; \
}

#define OBJECT_ARRAY_IMPLEMENT(TYPE) \
ObjectArray* ObjectArray_Create_##TYPE(sizetype capacity) { \
    mgdl_assert_print(capacity > 0, "Cannot create empty DynamicArray for " #TYPE); \
    ObjectArray* arr = (ObjectArray*)malloc(sizeof(DynamicArray)); \
    sizetype membytes = sizeof(TYPE) * capacity;\
    mgdl_assert_print(membytes > 0, "Cannot malloc 0 bytes for dynamic array" #TYPE); \
    arr->data = (void*)malloc(membytes); \
    arr->capacity = capacity; \
    arr->count = 0; \
    return arr; \
} \
TYPE* ObjectArray_Get_##TYPE ( ObjectArray* array, sizetype index ) { \
    if (index < array->count) { \
        TYPE* v = (TYPE*)array->data; \
        return &v[index]; \
    } else { \
        return nullptr; \
    } \
} \
sizetype ObjectArray_Add_##TYPE ( ObjectArray* array, TYPE item ) {\
    if (array->count + 1 > array->capacity) { \
        array->data = (void*)realloc(array->data, sizeof(TYPE) * array->capacity * 2); \
        array->capacity *= 2; \
    } \
    TYPE* v = (TYPE*)array->data; \
    v[array->count] = item; \
    array->count += 1; \
    return  array->count-1; \
} \
