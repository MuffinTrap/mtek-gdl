#include <mgdl/mgdl-splash.h>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-palette.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-debugfont.h>
#include <mgdl/mgdl-config.h>
#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-draw2d.h>

#include <string.h>

static float areaHeight;
static float lineHeight;
static float lineWidth;
static float areaTop;
static float areaBottom;
static float animationProgress = 0.0f;
static const char* holdMessage = "Hold A to start";


// lean is in pixels
// width is in [0, 1]
static void GetCorners(float xOffset, float start, float height, float* cornersOut, float lean, float width)
{
	width = clampF(width, 0.0f, 1.0f);

	// The lean needs to be the same regardless of segment height
	vec2 AreaTopLeft = vec2New(xOffset + lean, areaTop);
	vec2 AreaBottomLeft = vec2New(xOffset, areaBottom);

	vec2 downwards = vec2Add(AreaBottomLeft, vec2Negate(AreaTopLeft));
	// Lean vector points left and down
	vec2 D = vec2Normalize(downwards);

	// From area top left go down until segment starts
	vec2 tl = vec2Add(AreaTopLeft, vec2Multiply(D, (float)(start * lineHeight)));
	cornersOut[0] = tl.x;
	cornersOut[1] = tl.y;

	// From segment start go down * height
	vec2 bl = vec2Add(tl, vec2Multiply(D, (float)(height * lineHeight)));
	cornersOut[2] = bl.x;
	cornersOut[3] = bl.y;

	// Bottom right
	cornersOut[4] = bl.x + lineWidth * width;
	cornersOut[5] = bl.y;

	// Top right
	cornersOut[6] = tl.x + lineWidth * width;
	cornersOut[7] = tl.y;
}

// bars colored is how many bars should
// have color. Fractional value means bar is not fully coloured
static float DrawLetter(float x, u8* sl, u8 bars, u32 color, float lean, bool useBars, float barsColored)
{
	float startX = x;
	float corners[] = {0,0,0,0, 0,0,0,0};
	if (!useBars)
	{
		barsColored = bars;
	}

	for (int i = 0; i < bars; i++)
	{
		int line = i *2;
		if (barsColored > 0.0f)
		{
			GetCorners(x, sl[line+0], sl[line+1], corners, lean, barsColored);

			Draw2D_Quad(corners[0], corners[1], corners[2], corners[3],
						corners[4], corners[5], corners[6], corners[7],
						color);
			barsColored -= 1.0f;
		}
		x += lineWidth-1;
	}
	return x - startX;
}
// Returns where the version string should be
static float DrawLetters(float x, float lean, u32 color, bool useBars, float barsColored)
{
	// m
	// start points and lengths
	// start points 0, 1, 2,
	// lenghts : o height = 8
	u8 m_ls[] = {
		8, 1,
		8, 8,
		8, 1,
		9, 7,
		8, 1,
		9, 7,
		15, 1}; // leads to g

	// g in three parts
	u8 g_outer[] = {8,8,
		8, 1,
		8, 1,
		8, 16,
	};
	u8 g_under[] = {0,0,
		15, 1,
		15, 1,
		0,0};
	u8 gl_foot[] = {8+8+4, 4,
		23, 1,
		23, 1,
		0,0,
		8, 1  // Leads to d
	};

	// d in two parts
	u8 d_outer[] = {
		8,8,
		8,1,
		8,1,
		0, 16};

	u8 d_under[] = {
		0, 0,
		15, 1,
		15, 1,
		0,0,
		15, 1}; // leads to l

	u8 l_ls[] = {
		0, 16,
		15,1,
	};


	float drawX = x;
		drawX += DrawLetter(drawX, m_ls, 7, color, lean, useBars, barsColored);
		barsColored -= 7.0f;

		DrawLetter(drawX, g_outer, 4, color, lean, useBars, barsColored);
		DrawLetter(drawX, g_under, 4, color, lean, useBars, barsColored);
		drawX += DrawLetter(drawX, gl_foot, 5, color, lean, useBars, barsColored);
		barsColored -= 5.0f;
		drawX -= 1; // Prevent gaps between letters
		float dStart = drawX + lineWidth;

		DrawLetter(drawX, d_outer, 4, color, lean, useBars, barsColored);
		drawX += DrawLetter(drawX, d_under, 5, color, lean, useBars, barsColored);
		drawX -= 1;
		barsColored -= 5.0f;

		drawX += DrawLetter(drawX, l_ls, 2, color, lean, useBars, barsColored);
		barsColored -= 5.0f;
		return dStart;
}

// Returns the animation progress
float DrawSplashScreen(float deltaTime, bool drawHoldAMessage, float aHoldTimer)
{
	mgdl_glClear(GL_COLOR_BUFFER_BIT);
	// Draws mgdl
	// in stylized letters
	mgdl_InitOrthoProjection();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	Palette* blessing = Palette_GetDefault();
	Font* debf = Font_GetDebugFont();
	u32 yellow = Palette_GetColor(blessing, 3);
	u32 grey = Palette_GetColor(blessing, 6);


	const u16 sw = mgdl_GetScreenWidth();
	const u16 sh = mgdl_GetScreenHeight();

	// Logo takes up 6/4 of screen height
	areaHeight = (sh/6.0f) * 4.0f;
	// 3/4 screen width
	float areaWidth = (sw/4.0f) * 3.0f;

	// Space above and below logo
	float paddingY = (sh - areaHeight)/2.0f;
	areaBottom = paddingY;
	areaTop = areaBottom + areaHeight;

	// Logo is made of 19 segments
	float segments = 19.0f;
	lineWidth = (areaWidth) / segments;
	float oHeight = areaHeight / 3.0f;
	// Letter o is 8 lines tall
	// extensions are 8 lines in both directions
	lineHeight = oHeight / 8.0f;

	// Where the characters are drawn: ________________
	float baseLine = areaBottom + oHeight;

	// How much more the position on topline is to the right
	float lean = lineWidth * 3.0f;

	// Estimate of logowidth
	float logoWidth = lineWidth * segments + lean;

	// Space on both sides of logo
	float paddingX = (sw - logoWidth) / 2;

	float dStart = DrawLetters(paddingX, lean, grey, false, 0.0f);

	// Draw yellow over grey
	float barsColored = 0.0f;
	animationProgress += deltaTime * 0.5f;

	// Loop drawing
	{
		float drawProgress = animationProgress;
		while (drawProgress > 2.0f)
		{
			drawProgress -= 2.0f;
		}
		if (drawProgress <= 1.0f)
		{
			barsColored = segments * drawProgress;
		}
		else
		{
			// Color all yellow
			barsColored = segments;

		}
		DrawLetters(paddingX, lean, yellow, true, barsColored);

		// Draw grey over yellow
		if (drawProgress > 1.0f && drawProgress <= 2.0f)
		{
			barsColored = (drawProgress-1.0f) * segments;
			DrawLetters(paddingX, lean, grey, true, barsColored);
		}
	}
	Font_Print(debf, grey, dStart, baseLine, 8, GDL_VERSION);
	Font_Print(debf, grey, dStart, baseLine - 8, 8, Platform_GetSingleton()->name);

	if (drawHoldAMessage)
	{
		int messageWidth = strlen(holdMessage) * 8;
		int messageLeft = sw/2 - messageWidth/2;
		int messageY = areaBottom - 8;
		Font_Print(debf, yellow, messageLeft, messageY, 8, holdMessage);

		Draw2D_Rect(messageLeft, messageY - 16, messageLeft + messageWidth * aHoldTimer, messageY - 16 - 4, yellow);
	}
	return (animationProgress / 2.0f);
}
