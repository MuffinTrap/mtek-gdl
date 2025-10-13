#pragma once

#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-node.h>
#include <mgdl/mgdl-material.h>
#include <mgdl/mgdl-mesh.h>

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

// Different types needed by mtek-gdl

struct Material;
DynamicArray* DynamicArray_CreateMaterial(sizetype capacity);
sizetype DynamicArray_CountMaterial(DynamicArray* array);
Material* DynamicArray_GetMaterial(DynamicArray* array, sizetype index);
sizetype DynamicArray_AddMaterial(DynamicArray* array, Material* item);

#define DYNAMIC_ARRAY(TYPE) \
DynamicArray* DynamicArray_Create##TYPE(sizetype capacity); \
sizetype DynamicArray_Count##TYPE(DynamicArray* array); \
TYPE * DynamicArray_Get##TYPE(DynamicArray* array, sizetype index); \
sizetype DynamicArray_Add##TYPE(DynamicArray* array, TYPE * item);

// DYNAMIC_ARRAY(Material)
DYNAMIC_ARRAY(Mesh)
DYNAMIC_ARRAY(Node)


#undef DYNAMIC_ARRAY
