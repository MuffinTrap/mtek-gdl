#pragma once

#include "mgdl-wii.h"

/*! \addtogroup Basic Immediate mode gui
 *	\brief MenuCreator class for simple Immediate Mode Gui
 *
 *	\details This can be used to create simple menus with buttons and labels. Not much else yet. Added for version 0.100.0-muffintrap
 *
 *	@{
 */

namespace gdl
{
    class MenuCreator
    {
        public:
        MenuCreator();
        MenuCreator(gdl::FFont* font, float fontScale, float rowHeightEm);
        void StartMenu(int x, int y, int w, float cursorX, float cursorY, bool buttonPress);
        void SetColors(gdl::Color::ColorValues bg, gdl::Color::ColorValues border, gdl::Color::ColorValues text, gdl::Color::ColorValues highlight);
        void Panel(int h, u_int color);
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
        gdl::Color::ColorValues bg; 
        gdl::Color::ColorValues border; 
        gdl::Color::ColorValues text; 
        gdl::Color::ColorValues highlight;

        gdl::FFont* currentFont;
    };
};
