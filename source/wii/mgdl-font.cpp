// Font system module

#ifndef _GDL_WII_FONT
#define _GDL_WII_FONT

#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <gccore.h>
// muffintrap: Added <cstring> and <stdlib.h> to get strlen and aligned_alloc
// for version 0.100.0-muffintrap
#include <cstring>
#include <stdlib.h>

#include "mgdl-wii/mgdl-types.h"
#include "mgdl-wii/mgdl-globals.h"
#include "mgdl-wii/mgdl-globals-internal.h"
#include "mgdl-wii/mgdl-main.h"
#include "mgdl-wii/mgdl-image.h"
#include "mgdl-wii/mgdl-font.h"

// Fixed-sized font class functions

gdl::FFont::FFont() {

	cw=0;
	ch=0;
	vList=NULL;
	tList=NULL;
	firstIndex=0;
}

gdl::FFont::~FFont() {

	if (vList != NULL)
		free(vList);

	if (tList != NULL)
		free(tList);

}

// muffintrap: Added parameter firstCharacter
void gdl::FFont::BindSheet(gdl::Image& image, short charw, short charh,  char firstCharacter) {

	short	cx,cy;
	short	tx,ty;
	int		tc;

	charTexObj = image.Texture.TexObj();
	cw = charw;
	ch = charh;
	firstIndex = firstCharacter;
	short charactersPerRow = image.Texture.TXsize()/ charw;
	short rows = image.Texture.TYsize() / charh;
	short characterAmount = charactersPerRow * rows;

	if (gdl::ConsoleActive)
	{
		// This breaks all future printfs! :D
		printf("gdl: Binding a character font sheet. CPR:%d, R:%d, A:%d\n",
	charactersPerRow, rows, characterAmount);
	}

	//  muffintrap: Changed memalign() to aligned_alloc() for version 0.100.0-muffintrap
	if (vList == NULL)
		vList = aligned_alloc(32, sizeof(gdl::wii::VERT2s16)*4);

	((gdl::wii::VERT2s16*)vList)[0].x = 0;		((gdl::wii::VERT2s16*)vList)[0].y = 0;
	((gdl::wii::VERT2s16*)vList)[1].x = charw;	((gdl::wii::VERT2s16*)vList)[1].y = 0;
	((gdl::wii::VERT2s16*)vList)[2].x = charw;	((gdl::wii::VERT2s16*)vList)[2].y = charh;
	((gdl::wii::VERT2s16*)vList)[3].x = 0;		((gdl::wii::VERT2s16*)vList)[3].y = charh;

	DCFlushRange(vList, sizeof(gdl::wii::VERT2s16)*4);

	if (tList == NULL)
		tList = aligned_alloc(32, (sizeof(gdl::wii::TEX2f32)*4)*(characterAmount));

	for(cy=0; cy<rows; cy++) {
		for(cx=0; cx<charactersPerRow; cx++) {

			// Coordinates to source image
			tx = charw*cx;
			ty = charh*cy;

			// Texture coordinate array index
			tc = 4*(cx+(charactersPerRow*cy));

			// Upper-left
			((gdl::wii::TEX2f32*)tList)[tc].u	= ((f32)tx)/image.Texture.TXsize();
			((gdl::wii::TEX2f32*)tList)[tc].v	= ((f32)ty)/image.Texture.TYsize();

			// Upper-right
			// ((gdl::wii::TEX2f32*)tList)[tc+1].u	= ((f32)(tx+charw)-0.4f)/image.Texture.TXsize();
			((gdl::wii::TEX2f32*)tList)[tc+1].u	= ((f32)(tx+charw))/image.Texture.TXsize();
			((gdl::wii::TEX2f32*)tList)[tc+1].v	= ((f32)ty)/image.Texture.TYsize();

			// Lower-left
			// ((gdl::wii::TEX2f32*)tList)[tc+2].u	= ((f32)(tx+charw)-0.4f)/image.Texture.TXsize();
			((gdl::wii::TEX2f32*)tList)[tc+2].u	= ((f32)(tx+charw))/image.Texture.TXsize();
			((gdl::wii::TEX2f32*)tList)[tc+2].v	= (f32)(ty+charh)/image.Texture.TYsize();

			// Lower-right
			((gdl::wii::TEX2f32*)tList)[tc+3].u	= ((f32)tx)/image.Texture.TXsize();
			((gdl::wii::TEX2f32*)tList)[tc+3].v	= (f32)(ty+charh)/image.Texture.TYsize();
		}
	}

	DCFlushRange(tList, (sizeof(gdl::wii::TEX2f32)*4)*characterAmount);


	if (gdl::ConsoleActive)
		printf("Ok!\n");

}

void gdl::FFont::DrawText(const char *text, short x, short y, float scale, u32 col) {

	// Draws text using the current font set by gdl_SetCurrentFont()

	float	tx;
	int		c,tc;
	Mtx		TempMatrix;
	GXColor	TempCol;


	if (vList == NULL)
		return;


	if ((x == gdl::Centered) || (x == gdl::PCentered)) {
		tx = gdl::ScreenCenterX - ((((cw+1)*strlen(text))*scale)/2)+(scale/2);
	} else {
		tx = x;
	}

	TempCol.r = RED(col);
	TempCol.g = GREEN(col);
	TempCol.b = BLUE(col);
	TempCol.a = ALPHA(col);


	GX_LoadTexObj(charTexObj, GX_TEXMAP0);
	GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);

	GX_ClearVtxDesc();
	GX_SetVtxDesc(GX_VA_POS, GX_INDEX8);
	GX_SetVtxDesc(GX_VA_TEX0, GX_INDEX16);

	GX_SetArray(GX_VA_POS, vList, 2*sizeof(s16));
	GX_SetArray(GX_VA_TEX0, tList, 2*sizeof(f32));


	guMtxCopy(gdl::wii::ModelMtx, TempMatrix);
	guMtxApplyTrans(TempMatrix, TempMatrix, tx, y, 0);
	guMtxApplyScale(TempMatrix, TempMatrix, scale, scale, 0);
	GX_LoadPosMtxImm(TempMatrix, GX_PNMTX0);

	GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
	GX_SetChanMatColor(GX_COLOR0A0, TempCol);

	for(c=0; text[c] != 0x00; c++) {

		tc = 4*(((u_char*)text)[c] - firstIndex);

		GX_Begin(GX_QUADS, GX_VTXFMT0, 4);

			GX_Position1x8(0);
			GX_TexCoord1x16(tc);

			GX_Position1x8(1);
			GX_TexCoord1x16(tc+1);

			GX_Position1x8(2);
			GX_TexCoord1x16(tc+2);

			GX_Position1x8(3);
			GX_TexCoord1x16(tc+3);

		GX_End();

		guMtxApplyTrans(TempMatrix, TempMatrix, cw+1, 0, 0);
		GX_LoadPosMtxImm(TempMatrix, GX_PNMTX0);

	}

	GX_LoadPosMtxImm(gdl::wii::ModelMtx, GX_PNMTX0);
	GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);

}

void gdl::FFont::Printf(short x, short y, float scale, u32 col, const char* format, ... ) {

	va_list args;
	char	buff[256];

	va_start(args, format);
	vsprintf(buff, format, args);
	va_end(args);

	DrawText(buff, x, y, scale, col);

}

short gdl::FFont::GetHeight()
{
	return ch;
}

short gdl::FFont::GetWidth(const char* str)
{
	return strlen(str) * cw;
}

// muffintrap: removed Font class from version 0.100.0-muffintrap

#endif // _GDL_WII_FONT
