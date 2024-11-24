#pragma once

#include "mgdl-types.h"
#include "mgdl-image.h"
#include <string>

namespace gdl
{
	class Font
	{
	public:
		bool LoadFromBuffer(const u8* buffer, size_t size, short charw, short charh, char firstCharacter);
		bool LoadFromImage(const char* filename, short charw, short charh, char firstCharacter);
		bool LoadFromImage(const char* filename, short charw, short charh, char firstCharacter, short charactersPerRow);
		bool LoadFromImage(const char* filename, short charw, short charh, short charactersPerRow, std::string characters );

		// The text height is in units when drawing in 3D.
		// The text height is in pixels when drawing in 2D.
		void Printf(u32 color, short x, short y, float textHeight, const char* format, ... );
		void Print(u32 color, short x, short y, float textHeight, const char* text);

		void Printf(u32 color, short x, short y, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* format, ... );
		void Print(u32 color, short x, short y, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* text);

		void Printf(u32 color, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* format, ... );
		void Print(u32 color, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* text);

		void DrawSheet();
		void Bind(short charw, short charh, char firstCharacter);
		void Bind(short charw, short charh, char firstCharacter, short charactersPerRow);
		void Bind(short charw, short charh, std::string characters, short charactersPerRow);
		void SetSpacingOnce(float x, float y);

		// These are in pixels
		short GetCharacterWidth();
		short GetCharacterHeight();
	protected: // gdl::SpriteSet derives from this class

		gdl::Image fontImage;
		float spacingX = 0.0f;
		float spacingY = 0.0f;
		float aspect = 1.0f;

		void CreateTextureCoordList(short rows, short charactersPerRow, short texW, short texH);
		void CreateTextureCoordList(short rows, short charactersPerRow, short texW, short texH, std::string characters);
		gdl::vec2 GetTextureCoordinate(char character, char subIndex);
		GLuint textureName;
		short		cw,ch;
		gdl::vec2		*tList;
		char		firstIndex; // muffintrap: need to remember the first index to calculate offsets into vertex and uv arrays
	};
};
