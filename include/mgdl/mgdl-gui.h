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

/**
 * @brief Struct for holding the state of immeadiate mode gui menu.
 */
struct Menu
{
    float _drawx;
    float _drawy;
    float _menuWidth;
    float _textHeight;
    float _rowHeightEm;
    float _textSize;

    // Input state
    bool _buttonPress;
    bool _buttonHeld;
    vec2 _cursorPosition;

    // Colors
    Color4f _bg; /**< Background color of elements. */
    Color4f _text; /**< Color of the text, toggles and slider bars. */
    Color4f _highlight; /**< Used instead of _text when the item is hovered. */

    // Window
    bool _drawWindow;
    const char* _windowName;
    float _windowx;
    float _windowy;
    float _windowHeight;
    //float _windowWidth;

    Font* _font;
};
typedef struct Menu Menu;

#ifdef __cplusplus
extern "C"
{
#endif
    /**
     * @brief Draws a cursor using the Default font and default values.
     */
    void Menu_DrawCursor(void);

    void _Menu_DrawCursorParams(short x, short y, short w, short h, Color4f* color);

    /**
     * @brief Set the menu to which the other functions are applied to.
     * @details Only one menu is drawn at one time, so using this function saves typing.
     */
    void Menu_SetActive(Menu* menu);

    /**
     * @brief reads the default cursor and mouse status into the gui system
     */
    void Menu_ReadDefaultInputs(void);

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
    void Menu_StartInput(short x, short y, short width, vec2 cursorPosition, bool buttonPress, bool buttonHeld);

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
    void Menu_Start(short x, short y, short width);

    /**
     * @brief Sets the colors used in the menu.
     * @param bg Background color of labels and buttons.
     * @param text Color of the text.
     * @param highlight Highlight color to show hovered element.
     */
    void Menu_SetColors(Color4f* bg, Color4f* text, Color4f* highlight);

    /**
     * @brief Draws the window title bar
     */
    void _Menu_TitleBar(void);

    /**
     * @brief Draws the window borders
     */
    void _Menu_Borders(void);

    /**
     * @brief Draws a line separating elements.
     */
    void Menu_Separator(void);

    /**
     * @brief Leaves empty space for custom elements
     * @param height How much to move the position of next item downwards
     */
    void Menu_Skip(short height);
    /**
     * @brief Draws text.
     * @param text Text to be drawn.
     */
    void Menu_Text(const char* text);

    /**
     * @brief Draws formatted text.
     * @param text Format string.
     * @param __VA_ARGS__ Format parameters.
     */
    void Menu_TextF(const char* format, ...);

    /**
     * @brief Draws a colored icon
     * @param icon The icon to draw
     */
    void Menu_Icon(IconSymbol icon, Color4f* color);

    /**
     * @brief Draws a button that can be clicked.
     * @param text Text on the button.
     * @return True if the button was clicked.
     */
    bool Menu_Button(const char* text);
    /**
     * @brief Draws a toggle that can be clicked.
     * @param text Text on the toggle.
     * @param valueRef Pointer to the boolean controller by the toggle.
     * @return True if the toggle was clicked.
     */
    bool Menu_Toggle(const char* text, bool* valueRef);

    /**
     * @brief Draws a text label with can be highlighted.
     * @param text Text on the label.
     * @param enabled When true, the label uses highlight color
     */
    void Menu_Flag(const char* text, bool enabled);

    /**
     * @brief Draws a slider that can be adjusted with mouse or by pressing buttons.
     * @param text Text on the slider.
     * @param minValue The minimum value of the slider.
     * @param maxValue The maximum value of the slider.
     * @param valueRef Reference to the float value controlled by the slider.
     * @return True if the value was changed.
     */
    bool Menu_Slider(const char* text, float minValue, float maxValue, float* valueRef);

#ifdef __cplusplus
}
#endif
