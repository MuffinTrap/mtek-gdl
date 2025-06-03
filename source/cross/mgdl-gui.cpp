#include <mgdl/mgdl-gui.h>
#include <mgdl/mgdl-font.h>
#include <mgdl/mgdl-debugfont.h>
#include <mgdl/mgdl-draw2d.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-controller.h>
#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-main.h>


static Menu* menu = nullptr; /**< The active menu used. */
static bool mouseDown_ = false; /**< Default input state. */
static bool mousePress_ = false;
static vec2 cursorPosition_;

void Menu_ReadDefaultInputs()
{
    WiiController* c = Platform_GetController(0);
    cursorPosition_ = WiiController_GetCursorPosition(c);
    V2f_Y(cursorPosition_) = mgdl_GetScreenHeight() - V2f_Y(cursorPosition_);
    mouseDown_ = WiiController_ButtonHeld(c, ButtonA);
    mousePress_ = WiiController_ButtonPress(c, ButtonA);
}

void Menu_SetActive(Menu* active)
{
    menu = active;
}

Menu* Menu_CreateDefault()
{
    return Menu_Create(Font_GetDebugFont(), 1.0f, 1.1f);
}

Menu* Menu_Create(Font* font, float textHeight, float rowHeightEm)
{
    Menu* menu = new Menu();

    menu->_font = font;
    menu->_textHeight = textHeight;
    menu->_rowHeightEm = rowHeightEm;
    menu->_drawWindow = false;

    // TODO Calculate text and row heights just once

    // Default colors    //TODO Change to palette colors
    menu->_bg = gdl::Colors::Black;
    menu->_text = gdl::Colors::White;
    menu->_highlight = gdl::Colors::LightBlue;

    return menu;
}

Menu* Menu_CreateWindowed(Font* font, float textHeight, float rowHeightEm, short windowWidth, short windowHeight, const char* title)
{
    Menu* menu = Menu_Create(font, textHeight, rowHeightEm);
    menu->_drawWindow = true;
    menu->_windowName = title;
    menu->_windowWidth = windowWidth;
    menu->_windowHeight = windowHeight;
    // Invalid starting values
    menu->_windowx = -1;
    menu->_windowy = -1;

    return menu;
}


void Menu_Start(short x, short y, short width)
{
    Menu_StartInput(x, y, width, cursorPosition_, mousePress_, mouseDown_);
}

void Menu_StartInput(short x, short y, short width, vec2 cursorPosition, bool buttonPress, bool buttonHeld)
{
    if (menu != nullptr)
    {
        if (menu->_drawWindow)
        {
            if( menu->_windowx < 0)
            {
                menu->_windowx = x;
                menu->_windowy = y;
            }
            menu->_drawx = menu->_windowx;
            menu->_drawy = menu->_windowy;
            menu->_menuWidth = menu->_windowWidth;
            _Menu_Borders();
            _Menu_TitleBar();
        }
        else
        {
            menu->_drawx = x;
            menu->_drawy = y;
            menu->_menuWidth = width;
        }
        menu->_cursorPosition = cursorPosition;
        menu->_buttonPress = buttonPress;
        menu->_buttonHeld = buttonHeld;
    }

}

void _Menu_TitleBar()
{
    const short x = menu->_drawx;
    const short y = menu->_drawy;
    const short h = menu->_font->characterHeight * menu->_textHeight;
    DrawRectangle(x, y,
                  x + menu->_windowWidth,
                  y - h,
                  menu->_text);

    Font_Print(menu->_font, menu->_highlight, x + 2, y, h, menu->_windowName);

    menu->_drawy -= h;
}

void _Menu_Borders()
{
    const short x = menu->_drawx;
    const short y = menu->_drawy;
    const short h = menu->_font->characterHeight * menu->_textHeight;
    DrawRectangle(x, y,
                  x + menu->_windowWidth,
                  y - menu->_windowHeight,
                  menu->_bg);
    DrawRectangleLines(x, y,
                  x + menu->_windowWidth,
                  y - menu->_windowHeight,
                  menu->_highlight);

    DrawLine(0, 0, V2f_X(menu->_cursorPosition), V2f_Y(menu->_cursorPosition), Colors::White);
}

void Menu_SetColors(u32 bg, u32 text, u32 highlight)
{
    if (menu == nullptr) return;
        menu->_bg = bg;
        menu->_text = text;
        menu->_highlight = highlight;
}

void Menu_Panel(int h, u32 color)
{
    if (menu == nullptr) return;

        const short x = menu->_drawx;
        const short y = menu->_drawy;
        const short w = menu->_menuWidth;
        DrawRectangle(x, y, x + w, y - h, color);
        menu->_drawy -= h;
}

void Menu_TextF(const char* text, ...)
{
    MGDL_PRINTF_TO_BUFFER(text);
    Menu_Text(mgdl_GetPrintfBuffer());
}

void Menu_Text(const char* text)
{
    if (menu == nullptr) return;

    const short x = menu->_drawx;
    const short y = menu->_drawy;
    const short h = menu->_font->characterHeight * menu->_textHeight;
    Font_PrintAligned(menu->_font, menu->_text, x, y, h, LJustify, LJustify, text);
    menu->_drawy -= h * menu->_rowHeightEm;
}

void Menu_Icon(IconSymbol icon, rgba8 color)
{

    if (menu == nullptr) return;
    const short x = menu->_drawx;
    const short y = menu->_drawy;
    const short h = menu->_font->characterHeight * menu->_textHeight;
    Font_Icon(menu->_font, color, x, y, h, LJustify, LJustify, icon);
    menu->_drawy -= h * menu->_rowHeightEm;
}

bool Menu_Button(const char* text)
{
    if (menu == nullptr) return false;

    const short x = menu->_drawx;
    const short y = menu->_drawy;
    const short w = menu->_menuWidth;
    const short h = menu->_font->characterHeight * menu->_textHeight * menu->_rowHeightEm;

    const short cx = V2f_X(menu->_cursorPosition);
    const short cy = V2f_Y(menu->_cursorPosition);

    bool inside = ((cx >= x) &&
                (cx <= x + w) &&
                (cy <= y) &&
                (cy >= y - h));

    rgba8 c = menu->_bg;
    if (inside)
    {
        c = menu->_highlight;
    }
    DrawRectangle(x, y, x + w, y - h, c);

    // TODO Center text

    float textH = h/menu->_rowHeightEm;
    Font_PrintAligned(menu->_font, menu->_text, x, y, textH, LJustify, LJustify, text);
            
    menu->_drawy -= h ;

    return (inside && menu->_buttonPress);
}

bool Menu_Toggle ( const char* text, bool* valuePtr )
{
    if (menu == nullptr) return false;

    bool isOn = *valuePtr;

    const short x = menu->_drawx;
    const short y = menu->_drawy;
    const short w = menu->_menuWidth;
    const short h = menu->_font->characterHeight * menu->_textHeight * menu->_rowHeightEm;
    const short cx = V2f_X(menu->_cursorPosition);
    const short cy = V2f_Y(menu->_cursorPosition);

    bool inside = ((cx >= x) &&
                (cx <= x + w) &&
                (cy <= y) &&
                (cy >= y - h));

    short padding = 2;
    rgba8 c = menu->_bg;
    if (inside)
    {
        c = menu->_highlight;
    }
    DrawRectangleLines(x, y, x + w, y - h, c);

    if (isOn)
    {
        DrawRectangle(x + padding, y - padding,
                      x + h - padding, y - h + padding, menu->_text);
    }
    else
    {
        DrawRectangleLines(x + padding, y - padding,
                      x + h - padding, y - h + padding, menu->_text);
    }

    float textH = h/menu->_rowHeightEm;
    Font_PrintAligned(menu->_font, menu->_text, x + h + padding * 2, y, textH, LJustify, LJustify, text);

    menu->_drawy -= h + 1 ;

    if (inside && menu->_buttonPress)
    {
        *valuePtr = (!isOn);
    }

    return (inside && menu->_buttonPress);
}

void Menu_Flag(const char* text, bool enabled)
{
    const short x = menu->_drawx;
    const short y = menu->_drawy;
    const short w = menu->_menuWidth;
    const short h = menu->_font->characterHeight * menu->_textHeight * menu->_rowHeightEm;

    rgba8 c = menu->_bg;
    if (enabled)
    {
        c = menu->_highlight;
    }
    DrawRectangle(x, y, x + w, y - h, c);

    // TODO Center text

    float textH = h/menu->_rowHeightEm;
    Font_PrintAligned(menu->_font, menu->_text, x+w/2, y, textH, Centered, LJustify, text);

    menu->_drawy -= h ;
}


void Menu_Separator()
{
    // TODO
}
