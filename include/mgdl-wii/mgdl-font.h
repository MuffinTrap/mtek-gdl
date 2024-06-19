/*!	\file mgdl-font.h
 *	\brief Font system module header
 */
#ifndef _GDL_WII_FONT_H
#define _GDL_WII_FONT_H


namespace gdl {


/*!	\addtogroup fontModule Font Module
 *	\brief Class module for loading and drawing text fonts.
 *	@{
 */

//! Fixed font handling class
/*!
 *	Used for handling simple fixed-width fonts from a font sheet. A font sheet is essentially just an image with 256
 *	characters arranged in a 16x16 tile grid. The gdl::Font class however supports variable-width characters but is
 *	currently limited to supporting only DirectQB font files.
 */
class FFont {

	GXTexObj	*charTexObj;
	short		cw,ch;
	void		*vList;
	void		*tList;
	char		firstIndex; // muffintrap: need to remember the first index to calculate offsets into vertex and uv arrays

public:


	//! Class constructor.
	FFont();


	//! Class deconstructor.
	virtual	~FFont();


	// muffintrap: modified to be able to read in less than 256 characters. This way the usual
	// spritefont images that have chacters between " !"#$% ... xyz{|}~" can be used
	//! Binds the font handler class to an gdl::Image object of a font sheet.
	/*!
	 *	\details A font sheet consists of a simple array of 256 characters arranged in a 16x16 character grid. It is
	 *	good enough for simple fonts (such as those found in retro inspired games) but will become problematic with
	 *	dynamic fonts that have variable-width characters. If the sheet does not start from ASCII code 0,
	 *  the first character on the sheet can be specified
	 *
	 *	\param[in]	image	gdl::Image object of a font sheet.
	 *	\param[in]	charw	Width of characters in pixels.
	 *	\param[in]	charh	Height of characters in pixels.
	 *	\param[in]	firstCharacter	The first character in the image
	 */
	void BindSheet(gdl::Image& image, short charw, short charh, char firstCharacter);


	//! Draw text.
	/*!
	 *	\details Draws a string of text using the currently binded font sheet. Because it only accepts a pre-made
	 *	string of characters, it is not as flexible as printf() and gdl::FFont::Printf().
	 *
	 *	\param[in]	*text	Text to draw.
	 *	\param[in]	x		X coordinate of text (can be gdl::Centered to center text).
	 *	\param[in]	y		Y coordinate of text.
	 *	\param[in]	scale	Scale factor (1.0 is original size).
	 *	\param[in]	col		Color value (see gdl::Color::ColorValues or \ref colorMacros).
	 */
	void DrawText(const char *text, short x, short y, float scale, u32 col);


	//! Draw text with a printf-like syntax.
	/*!
	 *	\details Similar to gdl::FFont::DrawText() but is much more flexible as it accepts printf-style specifiers
	 *	and formatting parameters.
	 *
	 *	\note The overall length of the string must be no longer than 255 characters.
	 *
	 *	\param[in]	x		X coordinate of text (can be gdl::Centered to center text).
	 *	\param[in]	y		Y coordinate of text.
	 *	\param[in]	scale	Scale factor (1.0 is original size).
	 *	\param[in]	col		Color of text (see gdl::Color::ColorValues or \ref colorMacros).
	 *	\param[in]	*format	Text to draw supporting printf specifiers and formatting parameters.
	 *	\param[in]	...		Print arguments.
	 */
	void Printf(short x, short y, float scale, u32 col, const char* format, ... );

	// muffintrap: added this to get the height of the font
	//! Returns the height of this font in pixels.
	/*!
	 *
	 *	\returns Height of the font in pixels, zero if no font was loaded or the string is blank.
	 */
	short GetHeight();

	// muffintrap: added this to get the width of a single character
	//! Returns the width of this font in pixels.
	/*!
	 *
	 *	\returns width of the string in pixels, zero if no font was loaded or the string is blank.
	 */
	short GetCharacterWidth();

	// muffintrap: added this to get the width of the string in this font
	//! Returns the width of the given string in pixels.
	/*!
	 *
	 *	\param[in]	str		The string to be measured
	 *
	 *	\returns width of the string in pixels, zero if no font was loaded or the string is blank.
	 */
	short GetStringWidth(const char* str);
};

// muffintrap: removed Font class from version 0.100.0-muffintrap
// since I don't think DirectQB fonts get much use anymore.

/*!	@} */
}

#endif // _GDL_WII_FONT_H
