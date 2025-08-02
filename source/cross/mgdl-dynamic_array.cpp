#include <mgdl/mgdl-dynamic_array.h>
#include <mgdl/mgdl-assert.h>
#include <vector>

DynamicArray* DynamicArray_CreateMaterial(sizetype capacity)
{
    DynamicArray* arr = new DynamicArray();
    arr->data = (void*)malloc(sizeof(Material*) * capacity);
    arr->capacity = capacity;
    arr->count = 0;
    return arr;
}

sizetype DynamicArray_CountMaterial(DynamicArray* array)
{
    return array->count;
}

Material * DynamicArray_GetMaterial ( DynamicArray* array, sizetype index )
{
    if (index < array->count) { \
        Material** v = (Material**)array->data; \
        return v[index]; \
    } else { \
        return nullptr; \
    } \
}

sizetype DynamicArray_AddMaterial ( DynamicArray* array, Material* item )
{
    if (array->count + 1 > array->capacity) { \
        array->data = (void*)realloc(array->data, sizeof(Material*) * array->capacity * 2); \
        array->capacity *= 2; \
    } \
    Material** v = (Material**)array->data;
    v[array->count] = item; \
    array->count += 1; \
    return  array->count-1; \
}

#define DYNAMIC_ARRAY_IMPL(TYPE) \
DynamicArray* DynamicArray_Create##TYPE(sizetype capacity) { \
    mgdl_assert_print(capacity > 0, "Cannot create empty DynamicArray"); \
    DynamicArray* arr = new DynamicArray(); \
    arr->data = (void*)malloc(sizeof(TYPE*) * capacity); \
    arr->capacity = capacity; \
    arr->count = 0; \
    return arr; \
} \
sizetype DynamicArray_Count##TYPE(DynamicArray* array) { \
    return array->count; \
} \
TYPE* DynamicArray_Get##TYPE ( DynamicArray* array, sizetype index ) { \
    if (index < array->count) { \
        TYPE** v = (TYPE**)array->data; \
        return v[index]; \
    } else { \
        return nullptr; \
    } \
} \
sizetype DynamicArray_Add##TYPE ( DynamicArray* array, TYPE* item ) {\
    if (array->count + 1 > array->capacity) { \
        array->data = (void*)realloc(array->data, sizeof(TYPE*) * array->capacity * 2); \
        array->capacity *= 2; \
    } \
    TYPE** v = (TYPE**)array->data; \
    v[array->count] = item; \
    array->count += 1; \
    return  array->count-1; \
}

//DYNAMIC_ARRAY_IMPL(Material)
DYNAMIC_ARRAY_IMPL(Mesh)
DYNAMIC_ARRAY_IMPL(Node)

#undef DYNAMIC_ARRAY_IMPL



