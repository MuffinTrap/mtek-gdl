#pragma once

#include "mgdl-types.h"

/*! \addtogroup Basic Immediate mode gui
 *	\brief MenuCreator class for simple Immediate Mode Gui
 *
 *	\details This can be used to create simple menus with buttons and labels. Not much else yet. Added for version 0.100.0-muffintrap
 *
 *	@{
 */

namespace gdl
{
    class Font;
    class MenuCreator
    {
        public:
        MenuCreator();
        MenuCreator(gdl::Font* font, float fontScale, float rowHeightEm);
        void StartMenu(int x, int y, int w, float cursorX, float cursorY, bool buttonPress);
        void SetColors(u32 bg, u32 border, u32 text, u32 highlight);
        void Panel(int h, u32 color);
        void Text(const char* text);
        bool Button(const char* text);

        private:

        // Drawing parameters
        int x;
        int y;
        int w;
        float fontScale;
        float rowHeightEm;

        // Input state
        bool buttonPress;
        float cursorX;
        float cursorY;

        // Colors
        u32 bg; 
        u32 border; 
        u32 text; 
        u32 highlight;

        gdl::Font* currentFont;
    };
};
