#pragma once

#include "mgdl-types.h"
#include "mgdl-font.h"
#include "mgdl-color.h"

/**
 * @file mgdl-gui.h
 * @brief MenuCreator class for simple Immediate Mode Gui
 *
 * @details This can be used to create simple menus with buttons and labels. Not much else yet. Added for version 0.100.0-muffintrap
 *
 */

enum MenuDrawDirection
{
    MenuDownward,
    MenuRightward
};
typedef enum MenuDrawDirection MenuDrawDirection;

/**
 * @brief Struct for holding the state of immeadiate mode gui menu.
 */
struct Menu
{
    float drawx;
    float drawy;
    float menuWidth;
    float textHeight;
    float rowHeightEm;
    float textSize;

    // Input state
    bool buttonPress;
    bool buttonHeld;
    vec2 cursorPosition;

    // Colors
    Color4f bg; /**< Background color of elements. */
    Color4f text; /**< Color of the text, toggles and slider bars. */
    Color4f highlight; /**< Used instead of _text when the item is hovered. */

    // Window
    bool drawWindow;
    const char* windowName;
    float windowx;
    float windowy;
    float windowHeight;

    Font* font;

    MenuDrawDirection drawDirection;
    float largestHeightOnRow;
    float startx;
};
typedef struct Menu Menu;

#ifdef __cplusplus
extern "C"
{
#endif
    /**
     * @brief Draws a cursor using the Default font and default values.
     */
    void Menu_DrawCursor(Menu* menu);

    void Menu_DrawCursorParams_(short x, short y, short w, short h, Color4f* color);

    /**
     * @brief Set the menu to which the other functions are applied to.
     * @details Only one menu is drawn at one time, so using this function saves typing.
     */
    void Menu_SetActive(Menu* menu);

    /**
     * @brief reads the default cursor and mouse status into the menu
     */
    void Menu_ReadDefaultInputs(Menu* menu);

    /**
     * @brief Creates a MenuCreator using font and parameters.
     * @param font Font to use on the menu elements.
     * @param textHeight Height of the text.
     * @param rowHeightEm Height of the row relative to the text height. 1 Em is same as text height, 2 is twice etc. Controls the amount of padding above and below text on elements.
     * @return Menu struct that can be used to draw menus.
     */
    Menu* Menu_Create(Font* font, float textHeigh, float rowHeightEm);

    /**
     * @brief Creates a windowed menu.
     * @param font Font to use on the menu elements.
     * @param textHeight Height of the text.
     * @param rowHeightEm Height of the row relative to the text height. 1 Em is same as text height, 2 is twice etc. Controls the amount of padding above and below text on elements.
     * @return Menu struct that can be used to draw menus.
     * @param width Width of the window in pixels. -1 means automatic
     * @param height Height of the window in pixels. -1 means automatic
     * @param text Title of the window.
     */
    Menu* Menu_CreateWindowed(Font* font, float textHeight, float rowHeightEm, short width, short height, const char* title);

    /**
     * @brief Creates a MenuCreator using default font and settings.
     * @return Menu struct that can be used to draw menus.
     */
    Menu* Menu_CreateDefault(void);

    /**
     * @brief Starts the menu from given position and takes the input status.
     *
     * This function sets up the menu state but does not draw anything yet. It requires the cursor position and button state.
     *
     * @param x Upper left corner x of the first element.
     * @param y Upper left corner y of the first element.
     * @param width Width of the elements in the menu.
     * @param cursorX cursorPosition Position of the cursor on the screen in pixels.
     * @param buttonPress Is the button pressed on this frame.
     */
    void Menu_StartInput(Menu* menu, short x, short y, short width, vec2 cursorPosition, bool buttonPress, bool buttonHeld);

    /**
     * @brief Starts the menu from given position and uses default inputs
     *
     * @details This function sets up the menu state but does not draw anything yet. It uses the input status that was read when Menu_ReadDefaultInputs was last called.
     * If the menu is windowed, the x and y only set the starting position
     *
     * @param x Upper left corner x of the first element.
     * @param y Upper left corner y of the first element.
     * @param width Width of the elements in the menu.
     */
    void Menu_Start(Menu* menu, short x, short y, short width);

    /**
     * @brief Sets the colors used in the menu.
     * @param bg Background color of labels and buttons.
     * @param text Color of the text.
     * @param highlight Highlight color to show hovered element.
     */
    void Menu_SetColors(Menu* menu, Color4f* bg, Color4f* text, Color4f* highlight);

    void Menu_BeginRow(Menu* menu);
    void Menu_EndRow(Menu* menu);

    /**
     * @brief Draws the window title bar
     */
    void Menu_TitleBar_(Menu* menu);

    /**
     * @brief Draws the window borders
     */
    void Menu_Borders_(Menu* menu);

    /**
     * @brief Tells if cursor is inside the next area of this menu
     * @param menu The menu to use
     * @param w Width of the next area
     * @param h Height of the next area
     * @return True when cursor is inside
     */
    bool Menu_IsCursorInside(Menu* menu, short w, short h);


    /**
     * @brief Leaves empty space for custom elements
     * @param height How much to move the position of next item downwards
     */
    void Menu_Skip(Menu* menu, short pixels);
    /**
     * @brief Draws text.
     * @param text Text to be drawn.
     */
    void Menu_Text(Menu* menu, const char* text);

    /**
     * @brief Draws formatted text.
     * @param text Format string.
     * @param __VA_ARGS__ Format parameters.
     */
    void Menu_TextF(Menu* menu, const char* format, ...);

    /**
     * @brief Draws a colored icon
     * @param icon The icon to draw
     */
    void Menu_Icon(Menu* menu, IconSymbol icon, Color4f* color);

    /**
     * @brief Draws a button that can be clicked.
     * @param text Text on the button.
     * @return True if the button was clicked.
     */
    bool Menu_Button(Menu* menu, const char* text);

    /**
     * @brief Draws a button with a texture that can be clicked.
     * @param text Texture on the button
     * @param flipflags Combination of TextureFlipModes flags
     * @return True if the button was clicked.
     */
    bool Menu_TexturedButton(Menu* menu, Texture* texture, TextureFlipModes flipflags);
    /**
     * @brief Draws a toggle that can be clicked.
     * @param text Text on the toggle.
     * @param valueRef Pointer to the boolean controller by the toggle.
     * @return True if the toggle was clicked.
     */
    bool Menu_Toggle(Menu* menu, const char* text, bool* valueRef);

    /**
     * @brief Draws a text label with can be highlighted.
     * @param text Text on the label.
     * @param enabled When true, the label uses highlight color
     */
    void Menu_Flag(Menu* menu, const char* text, bool enabled);

    /**
     * @brief Draws a slider that can be adjusted with mouse or by pressing buttons.
     * @param text Text on the slider.
     * @param minValue The minimum value of the slider.
     * @param maxValue The maximum value of the slider.
     * @param valueRef Reference to the float value controlled by the slider.
     * @return True if the value was changed.
     */
    bool Menu_Slider(Menu* menu, const char* text, float minValue, float maxValue, float* valueRef);

#ifdef __cplusplus
}
#endif
