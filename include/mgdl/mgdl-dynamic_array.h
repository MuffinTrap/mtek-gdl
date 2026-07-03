#pragma once

#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-memory.h>

/**
 * @file mgdl-dynamic_array.h
 * @ingroup core
 * @brief DynamicArray types
 */

/**
 * @brief Struct for dynamic arrays
 * @note This same struct is used for both ObjectArray and PointerArray
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

/**
 * @brief Declare a new pointer array type. Pointer arrays store pointers
 * to objects stored somewhere else
 * @note Use this macro in a header file.
 * @param TYPE The type of the pointer.
 */
#define POINTER_ARRAY_DECLARE(TYPE) \
PointerArray* PointerArray_Create_##TYPE(sizetype capacity); \
TYPE * PointerArray_Get_##TYPE(PointerArray* array, sizetype index); \
sizetype PointerArray_Add_##TYPE(PointerArray* array, TYPE* item); \

/**
 * @brief Declare a new object array type. Object arrays store objects directly.
 * @note Use this macro in a header file.
 * @param TYPE The type of the object.
 */
#define OBJECT_ARRAY_DECLARE(TYPE) \
ObjectArray* ObjectArray_Create_##TYPE(sizetype capacity); \
TYPE * ObjectArray_Get_##TYPE(ObjectArray* array, sizetype index); \
sizetype ObjectArray_Add_##TYPE(ObjectArray* array, TYPE item);

/**
 * @brief Implement previously declared pointer array type.
 * @note Use this macro in a .cpp file
 * @param TYPE Type of the pointer.
 */
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

/**
 * @brief Implement previously declared pointer array type.
 * @note Use this macro in a .cpp file
 * @param TYPE Type of the pointer.
 */
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
