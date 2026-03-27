#include <mgdl/mgdl-dynamic_array.h>
#include <vector>

DynamicArray* DynamicArray_CreateMaterial(sizetype capacity)
{
    mgdl_assert_print(capacity > 0, "Cannot create empty DynamicArray for Material"); \
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

