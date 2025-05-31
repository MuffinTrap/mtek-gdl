#include <mgdl/mgdl-gui.h>
#include <mgdl/mgdl-font.h>
#include <mgdl/mgdl-debugfont.h>
#include <mgdl/mgdl-draw2d.h>
#include <mgdl/mgdl-util.h>


static Menu* menu = nullptr; /**< The active menu used. */

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

    // Default colors    //TODO Change to palette colors
    menu->_bg = gdl::Colors::Black;
    menu->_text = gdl::Colors::White;
    menu->_highlight = gdl::Colors::LightBlue;

    return menu;
}

void Menu_Start(short x, short y, short width, vec2 cursorPosition, bool buttonPress)
{
    if (menu != nullptr)
    {
        menu->_drawx = x;
        menu->_drawy = y;
        menu->_menuWidth = width;
        menu->_cursorPosition = cursorPosition;
        menu->_buttonPress = buttonPress;
    }
}

void Menu_SetColors(u32 bg, u32 text, u32 highlight)
{
    if (menu != nullptr)
    {
        menu->_bg = bg;
        menu->_text = text;
        menu->_highlight = highlight;
    }
}

void Menu_Panel(int h, u32 color)
{
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
    const short x = menu->_drawx;
    const short y = menu->_drawy;
    const short h = menu->_font->characterHeight * menu->_textHeight * menu->_rowHeightEm;
    Font_PrintAligned(menu->_font, menu->_text, x, y, h/menu->_rowHeightEm, LJustify, LJustify, text);
    menu->_drawy -= h;
}

bool Menu_Button(const char* text)
{
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

bool Menu_Toggle ( const char* text, bool* valueRef )
{
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

    if (valueRef)
    {
        DrawRectangle(x + padding, y - padding, x + h - padding, y - h + padding, menu->_text);
    }

    float textH = h/menu->_rowHeightEm;
    Font_PrintAligned(menu->_font, menu->_text, x + h + padding * 2, y, textH, LJustify, LJustify, text);

    menu->_drawy -= h ;

    if (inside && menu->_buttonPress)
    {
        *valueRef = !valueRef;
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
