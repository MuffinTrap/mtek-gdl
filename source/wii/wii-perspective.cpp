#include <ogc/gx.h>

#include "mgdl-wii/mgdl-wii-perspective.h"

gdl::Camera gdl::Perspective::defaultCamera = gdl::Camera();
int gdl::Perspective::stackTop = -1;
std::vector<gdl::MatrixContainer> gdl::Perspective::modelMatrixStack = std::vector<gdl::MatrixContainer>();


void gdl::Perspective::InitMatrixStack()
{
	modelMatrixStack.reserve(4);
	stackTop = -1;
}

void gdl::Perspective::SetDefaultVertexAttributes()
{
    // Set up a vertex format, which is used later in drawing
    // All attributes can be set, but only those that are
    // set can be mentioned in the GX_SetVtxDesc() calls

    // Format 1:
    // position as XYZ, each a 32 bit float.  0 for fixed point format (frac)
    GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_POS, GX_POS_XYZ,	GX_F32, 0);
    //  color as RGB, 8 bits per color
	GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_CLR0, GX_CLR_RGB, GX_RGB8, 0);

    // : normals as XYZ, each a 32 bit float.
	GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_NRM, GX_NRM_XYZ,	GX_F32,	0);
    // : texture coordinates as ST, both 32 bit floats.
	GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
}

void gdl::Perspective::SetTexture ( ImageWii& texture )
{
	GX_LoadTexObj(texture.Texture.TexObj(), GX_TEXMAP0);
}

void gdl::Perspective::InitDefaultCamera()
{
	defaultCamera.position = guVector{0.0f, 0.0f, 0.0f};
	defaultCamera.direction = gdl::FORWARD;
}

gdl::Camera & gdl::Perspective::GetDefaultCamera()
{
	return defaultCamera;
}

void gdl::Perspective::ApplyDefaultCamera()
{
	// TODO calculate up and forward from camera's rotation
	guVector cameraUp = gdl::UP;
	guVector cameraPos = defaultCamera.position;
	guVector target;
	guVecAdd(&cameraPos, &defaultCamera.direction, &target);
	guLookAt(gdl::wii::ViewMtx, &cameraPos, &cameraUp, &target);
}


