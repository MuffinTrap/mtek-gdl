// Sprite system module functions

#ifndef _GDL_WII_SPRITES
#define _GDL_WII_SPRITES

#include <stdio.h>
#include <malloc.h>
// muffintrap: added <cstring> for version 2.6.1
#include <cstring>

#include "mgdl-wii/mgdl-types.h"
#include "mgdl-wii/mgdl-globals.h"
#include "mgdl-wii/mgdl-globals-internal.h"
#include "mgdl-wii/mgdl-main.h"
#include "mgdl-wii/mgdl-image.h"
#include "mgdl-wii/mgdl-sprites.h"


// External declarations to be able to access the image module's put routines
namespace gdl {

//extern void _put(short x, short y, u_int col, GXTexObj *texObj, void *vList, void *tList);
extern void _put(short x, short y, short cx, short cy, short px, short py, float angle, float scale, u_int col, short xSize, short ySize, GXTexObj *texObj, void *vList, void *tList);
extern void _putS(short x1, short y1, short x2, short y2, u_int col, short xSize, short ySize, GXTexObj *texObj, void *vList, void *tList);

}


// Class functions

gdl::SpriteSet::SpriteSet() {

	sheetList=NULL;
	spriteList=NULL;
	numSprites=0;
	numSheets=0;

}

gdl::SpriteSet::~SpriteSet() {

	// Deallocate sprite lists if allocated
	if (spriteList != NULL) {
		short i;
		for(i=numSprites-1; i>=0; i--) {
			free(spriteList[i].vList);
			free(spriteList[i].tList);
		}
		free(spriteList);
		spriteList=NULL;
	}


	// Deallocate sheet images if allocated
	if (sheetList != NULL) {
		short i;
		for(i=numSheets-1; i>=0; i--) delete sheetList[i];
		free(sheetList);
		sheetList=NULL;
	}

	numSprites=0;
	numSheets=0;

}

gdl::SpriteSetConfig gdl::SpriteSet::CreateConfig(short numSprites, short tileRows, short tilesPerRow, short tileWidth, short tileHeight)
{
	SpriteSetConfig cfg = {0};
	 cfg.numSprites= numSprites;
	 cfg.tileRows= tileRows; 
	 cfg.tilesPerRow= tilesPerRow;
	 cfg.tileWidth= tileWidth;
	 cfg.tileHeight= tileHeight;
	return cfg;
}
/*
	Change 13.4.2024
	muffintrap: added new function that loads from image and configuration
*/
bool gdl::SpriteSet::LoadSprites(SpriteSetConfig &config, Image *spriteSheet)
{
	if (spriteSheet == NULL) 
	{
		return false;
	}
	if (config.tilesPerRow * config.tileRows != config.numSprites)
	{
		return false;
	}
	numSprites	= config.numSprites;
	numSheets	= 1;
	spriteList	= (Sprite*)calloc(numSprites, sizeof(Sprite));
	sheetList	= (Image**)calloc(1, sizeof(Image*));
	sheetList[0] = spriteSheet;


	TSM_entry entry = {0};
	// NOTE! Sheet numbering starts from 1
	entry.sheetnum = 1;

	for (short i = 0; i < config.numSprites; i++)
	{
		short column = i % config.tilesPerRow;
		short row = i / config.tilesPerRow;

		entry.tx1 = column * config.tileWidth;
		entry.tx2 = entry.tx1 + config.tileWidth;
		entry.ty1 = row * config.tileHeight;
		entry.ty2 = entry.ty1 + config.tileHeight;
		// Pivot point?
		entry.px = entry.tx1 + config.tileWidth/2;
		entry.py = entry.ty1 + config.tileHeight/2;

		LoadTSM_Entry(entry, i);
	}

	if (gdl::ConsoleActive)
		printf("gdl: Sprite sheets loaded successfully.\n");

	// Clear sheetList since the Image is freed somewhere else
	sheetList = NULL;

	return(true);
}

bool gdl::SpriteSet::LoadSprites(const char *fileName, const char *sheetsdir, u_int filtermode, u_int format) {

	// Open up the file
	FILE *fp;

	if (gdl::ConsoleActive) printf("gdl: Loading sprites in file %s...", fileName);

	if (!(fp = fopen(fileName, "r"))) {
		gdl::CallErrorCallback("Sprite map file %s not found.", fileName);
		return(false);
	}


	// Check the header
	struct {
		char	id[3];
		char	ver;
		short	numSheets;
		short	numSprites;
	} TSM_header;

	fread(&TSM_header, 1, sizeof(TSM_header), fp);
	gdl::wii::RevBytes(&TSM_header.numSheets, sizeof(short));
	gdl::wii::RevBytes(&TSM_header.numSprites, sizeof(short));

	if (strncmp(TSM_header.id, "TSM", 3)) {
		gdl::CallErrorCallback("%s not a TSM sprite map file.", fileName);
		fclose(fp);
		return(false);
	}

	if (TSM_header.ver != 0) {
		gdl::CallErrorCallback("Format version of %s is not supported.", fileName);
		fclose(fp);
		return(false);
	}

	if (gdl::ConsoleActive)
		printf("Ok!\n");


	// Set important private variables and then allocate sprite and sheet lists
	numSprites	= TSM_header.numSprites;
	numSheets	= TSM_header.numSheets;
	spriteList	= (Sprite*)calloc(TSM_header.numSprites, sizeof(Sprite));
	sheetList	= (Image**)calloc(TSM_header.numSheets, sizeof(Image*));


	// Load the sprite sheet images
	if (gdl::ConsoleActive) printf("gdl: Now attempting to load %d sprite sheet(s).\n", TSM_header.numSheets);

	short	i;
	char	namelen,namebuff[64];
	char	pathbuff[256];

	for(i=0; i<TSM_header.numSheets; i++) {

		fread(&namelen, 1, 1, fp);
		fread(namebuff, 1, namelen, fp);
		namebuff[(short)namelen] = 0x00;

		strcpy(pathbuff, sheetsdir);
		strcat(pathbuff, namebuff);

		sheetList[i] = new Image;
		if (!(sheetList[i]->LoadImage(pathbuff, filtermode, format))) {

			for (short d=i-1; d>=0; d--) delete sheetList[d];
			for (short d=i-1; d>=0; d--) {
				delete spriteList[d].vList;
				delete spriteList[d].tList;
			}

			free(spriteList);	spriteList=NULL;
			free(sheetList);	sheetList=NULL;
			return(false);

		}

	}


	/*
		Change 13.4.2024
		muffintrap: Changed to use function LoadTMS_entry()
	*/
	TSM_entry entry = {0};

	for(i=0; i<TSM_header.numSprites; i++) {

		// Read an entry and then flip its bytes around to make it BE
		fread(&entry, 1, sizeof(entry), fp);
		gdl::wii::RevBytes(&entry.sheetnum, sizeof(short));
		gdl::wii::RevBytes(&entry.tx1, sizeof(short));
		gdl::wii::RevBytes(&entry.ty1, sizeof(short));
		gdl::wii::RevBytes(&entry.tx2, sizeof(short));
		gdl::wii::RevBytes(&entry.ty2, sizeof(short));
		gdl::wii::RevBytes(&entry.px, sizeof(short));
		gdl::wii::RevBytes(&entry.py, sizeof(short));


		LoadTSM_Entry(entry, i);
	}


	// Finish off the rest
	fclose(fp);

	if (gdl::ConsoleActive)
		printf("gdl: Sprite sheets loaded successfully.\n");

	return(true);

}

void gdl::SpriteSet::LoadTSM_Entry(TSM_entry &entry, short index)
{
		// Calculate sprite width and height and set the sprite's center coords
		spriteList[index].w = (entry.tx2 - entry.tx1) + 1;
		spriteList[index].h = (entry.ty2 - entry.ty1) + 1;
		spriteList[index].cx = entry.px;
		spriteList[index].cy = entry.py;

		// Generate vertex list
		spriteList[index].vList = (wii::VERT2s16*)memalign(32, sizeof(wii::VERT2s16)*4);
		spriteList[index].vList[0].x = 0;					
		spriteList[index].vList[0].y = 0;
		spriteList[index].vList[1].x = spriteList[index].w;	
		spriteList[index].vList[1].y = 0;
		spriteList[index].vList[2].x = spriteList[index].w;	
		spriteList[index].vList[2].y = spriteList[index].h;
		spriteList[index].vList[3].x = 0;					
		spriteList[index].vList[3].y = spriteList[index].h;
		DCFlushRange(spriteList[index].vList, sizeof(wii::VERT2s16)*4);

		// Generate texcoord list
		entry.tx2++;
		entry.ty2++;
		spriteList[index].tList = (wii::TEX2f32*)memalign(32, sizeof(wii::TEX2f32)*4);
		short tWidth = sheetList[entry.sheetnum-1]->Texture.TXsize();
		short tHeight = sheetList[entry.sheetnum-1]->Texture.TYsize();
		spriteList[index].tList[0].u = ((float)entry.tx1+0.01f)	/ tWidth;
		spriteList[index].tList[0].v = ((float)entry.ty1+0.01f)	/ tHeight;
		spriteList[index].tList[1].u = (float)entry.tx2			/ tWidth;
		spriteList[index].tList[1].v = ((float)entry.ty1+0.01f)	/ tHeight;
		spriteList[index].tList[2].u = (float)entry.tx2			/ tWidth;
		spriteList[index].tList[2].v = (float)entry.ty2			/ tHeight;
		spriteList[index].tList[3].u = ((float)entry.tx1+0.01f)	/ tWidth;
		spriteList[index].tList[3].v = (float)entry.ty2			/ tHeight;
		DCFlushRange(spriteList[index].tList, sizeof(wii::VERT2s16)*4);

		// Set appropriate texture object
		spriteList[index].texObj = sheetList[entry.sheetnum-1]->Texture.TexObj();
}

short gdl::SpriteSet::NumSprites() {

	return(numSprites);

}

const gdl::Sprite *gdl::SpriteSet::SpriteInfo(short index) {

	return(&spriteList[index]);

}

void gdl::SpriteSet::Put(short x, short y, int index, u_int col, short cx, short cy, float scale, float angle) {

	_put(x, y, cx, cy, spriteList[index].cx, spriteList[index].cy, angle, scale, col,
		spriteList[index].w, spriteList[index].h, spriteList[index].texObj, spriteList[index].vList, spriteList[index].tList);

}

void gdl::SpriteSet::PutS(short x1, short y1, short x2, short y2, short index, u_int col) {

	_putS(x1, y1, x2, y2, col, spriteList[index].w, spriteList[index].h,
		spriteList[index].texObj, spriteList[index].vList, spriteList[index].tList);

}


#endif // _GDL_WII_SPRITES
