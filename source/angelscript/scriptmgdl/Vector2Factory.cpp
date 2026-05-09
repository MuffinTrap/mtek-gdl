#include "Vector2Factory.h"
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-alloc.h>

static Vector2* vectors= nullptr;
static u32 vectorCount;

void InitVector2Factory(u32 amountStored)
{
	if (vectors == nullptr)
	{
		vectors = (Vector2*)mgdl_AllocateGeneralMemory(sizeof(Vector2)*amountStored);
		vectorCount = amountStored;
	}
}
