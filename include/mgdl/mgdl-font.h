#pragma once

#include "mgdl-types.h"
#include "mgdl-image.h"

namespace gdl
{
	class Font
	{
	public:
		bool LoadFromBuffer(const u8* buffer, size_t size, short charw, short charh, char firstCharacter);
		bool LoadFromImage(const char* filename, short charw, short charh, char firstCharacter);
		void Printf(u32 color, float scale, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* format, ... );
		void Print(u32 color, float scale, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* text);
		void DrawSheet();
		void Bind(short charw, short charh, char firstCharacter);
		void SetSpacingOnce(float x, float y);
		short GetCharacterWidth();
		short GetCharacterHeight();
	protected: // gdl::SpriteSet derives from this class

		gdl::Image fontImage;
		float spacingX;
		float spacingY;
		float aspect;

		void CreateTextureCoordList(short rows, short charactersPerRow, short texW, short texH);
		gdl::vec2 GetTextureCoordinate(char character, char subIndex);
		GLuint textureName;
		short		cw,ch;
		gdl::vec2		*tList;
		char		firstIndex; // muffintrap: need to remember the first index to calculate offsets into vertex and uv arrays
	};
};
