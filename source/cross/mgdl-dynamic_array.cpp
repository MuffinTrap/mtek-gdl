#include <mgdl/mgdl-dynamic_array.h>

sizetype DynamicArray_Count(DynamicArray* array)
{
	return array->count;
}
void* DynamicArray_Get(DynamicArray* array, sizetype index)
{
	if (index < array->count)
	{
		return &((void**)array->data)[index];
	}
	else
	{
		return nullptr;
	}
}

