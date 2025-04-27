#pragma once

#include "mgdl-types.h"
#include "mgdl-font.h"

/**
 * @file mgdl-gui.h
 * @brief MenuCreator class for simple Immediate Mode Gui
 *
 * @details This can be used to create simple menus with buttons and labels. Not much else yet. Added for version 0.100.0-muffintrap
 *
 */

namespace gdl
{
    /**
     * @brief Class for creating immediate mode menus.
     */
    class MenuCreator
    {
        public:
            /**
             * @brief Creates a MenuCreator using the debug font and default values.
             */
        MenuCreator();
        /**
         * @brief Creates a MenuCreator using font and parameters.
         * @param font Font to use on the menu elements.
         * @param textHeight Height of the text.
         * @param rowHeightEm Height of the row relative to the text height. 1 Em is same as text height, 2 is twice etc. Controls the amount of padding above and below text on elements.
         */
        MenuCreator(Font* font, float textHeigh, float rowHeightEm);

        /**
         * @brief Starts the menu from given position and takes the input status.
         *
         * This function sets up the menu state but does not draw anything yet. It requires the cursor position and button state.
         *
         * @param x Upper left corner x of the first element.
         * @param y Upper left corner y of the first element.
         * @param cursorX X of the cursor.
         * @param cursorY Y of the cursor.
         * @param buttonPress Is the button pressed on this frame.
         */
        void StartMenu(int x, int y, int w, float cursorX, float cursorY, bool buttonPress);

        /**
         * @brief Sets the colors used in the menu.
         * @param bg Background color of labels and buttons.
         * @param border Border color of buttons.
         * @param text Color of the text.
         * @param highlight Highlight color to show hovered element.
         */
        void SetColors(rgba8 bg, rgba8 border, rgba8 text, rgba8 highlight);


        /**
         * @brief Draws an empty panel filled with color.
         * @param h Height of the panel.
         * @param color Color of the panel.
         */
        void Panel(int h, rgba8 color);
        /**
         * @brief Draws text.
         * @param text Text to be drawn.
         */
        void Text(const char* text);

        void TextF(const char* text, ...);

        /**
         * @brief Draws a button that can be clicked.
         * @param text Text on the button
         * @return True if the button was clicked.
         */
        bool Button(const char* text);
        /**
         * @brief Draws a toggle that can be clicked.
         * @param text Text on the toggle.
         * @param valueRef Reference to the boolean controller by the toggle.
         * @return True if the toggle was clicked.
         */
        bool Toggle(const char* text, bool &valueRef);

        private:

        // Drawing parameters
        float x;
        float y;
        float w;
        float textHeight;
        float rowHeightEm;

        // Input state
        bool buttonPress;
        float cursorX;
        float cursorY;

        // Colors
        rgba8 bg;
        rgba8 border;
        rgba8 text;
        rgba8 highlight;

        Font* font;
    };
};
