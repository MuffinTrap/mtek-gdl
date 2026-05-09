#include <mgdl/mgdl-types.h>

u16 Handle_Index(Handle handle)
{
	return (u16)((u32)handle & 0xFFFF);
}
AssetType Handle_Type(Handle handle)
{
	return (AssetType)((u32)handle >> 16);
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
	return (TextureHandle)CreateHandle(Type_Sound, index);
}
PaletteHandle Handle_CreatePalette(u16 index)
{
	return (TextureHandle)CreateHandle(Type_Palette, index);
}
ImageHandle Handle_CreateImage(u16 index)
{
	return (TextureHandle)CreateHandle(Type_Image, index);
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

RectF Rect_CreateV2f(vec2 position, vec2 size)
{
	return {V2f_X(position), V2f_Y(position), V2f_X(size), V2f_Y(size)};
}
