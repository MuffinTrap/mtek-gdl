#include <mgdl/mgdl-types.h>

u16 Handle_Index(Handle handle)
{
	return (u16)((u32)handle & 0xFFFF);
}
AssetType Handle_Type(Handle handle)
{
	return (AssetType)((u32)handle >> 16);
}

bool Handle_IsValid(Handle handle)
{
	return (u32)handle != MGDL_INVALID_HANDLE;
}

static u32 CreateHandle(AssetType assetType, u16 index)
{
	return (u32)((u32)assetType << 16 | (u32)index);
}

TextureHandle Handle_CreateTexture(u16 index)
{
	return (TextureHandle)CreateHandle(Type_Texture, index);
}
SoundHandle Handle_CreateSound(u16 index)
{
	return (SoundHandle)CreateHandle(Type_Sound, index);
}
PaletteHandle Handle_CreatePalette(u16 index)
{
	return (PaletteHandle)CreateHandle(Type_Palette, index);
}
ImageHandle Handle_CreateImage(u16 index)
{
	return (ImageHandle)CreateHandle(Type_Image, index);
}

SceneHandle Handle_CreateScene(u16 index)
{
	return (SceneHandle)CreateHandle(Type_Scene, index);
}
ModelHandle Handle_CreateModel(u16 index)
{
	return (ModelHandle)CreateHandle(Type_Model, index);
}

RectF RectF_Create(float x, float y, float w, float h)
{
	RectF r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	return r;
}

RectF Rect_CreateV2f(Vector2 position, Vector2 size)
{
	return {(position.x), (position.y), (size.x), (size.y)};
}
