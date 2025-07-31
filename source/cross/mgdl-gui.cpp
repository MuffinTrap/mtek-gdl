#include <mgdl/mgdl-gui.h>
#include <mgdl/mgdl-font.h>
#include <mgdl/mgdl-defaultfont.h>
#include <mgdl/mgdl-draw2d.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-controller.h>
#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-palette.h>
#include <mgdl/mgdl-main.h>


static Menu* menu = nullptr; /**< The active menu used. */
static bool mouseDown_ = false; /**< Default input state. */
static bool mousePress_ = false;
static vec2 cursorPosition_;

void Menu_ReadDefaultInputs()
{
    WiiController* c = Platform_GetController(0);
    cursorPosition_ = WiiController_GetCursorPosition(c);
    mouseDown_ = WiiController_ButtonHeld(c, ButtonA);
    mousePress_ = WiiController_ButtonPress(c, ButtonA);
}

void Menu_SetActive(Menu* active)
{
    menu = active;
}

Menu* Menu_CreateDefault()
{
    return Menu_Create(DefaultFont_GetDefaultFont(), 1.0f, 1.1f);
}

Menu* Menu_Create(Font* font, float textHeight, float rowHeightEm)
{
    Menu* menu = (Menu*)malloc(sizeof(Menu));

    menu->_font = font;
    menu->_textHeight = textHeight;
    menu->_rowHeightEm = rowHeightEm;
    menu->_textSize = textHeight * font->characterHeight;
    menu->_drawWindow = false;

    // TODO Calculate text and row heights just once

    // Default colors    //TODO Change to palette colors
    Palette* blessing = Palette_GetDefault();

    menu->_bg = Palette_GetColor4f(blessing, 1);
    menu->_text = Palette_GetColor4f(blessing, 5);
    menu->_highlight = Palette_GetColor4f(blessing, 3);

    return menu;
}

Menu* Menu_CreateWindowed(Font* font, float textHeight, float rowHeightEm, short windowWidth, short windowHeight, const char* title)
{
    Menu* menu = Menu_Create(font, textHeight, rowHeightEm);
    menu->_drawWindow = true;
    menu->_windowName = title;
    menu->_menuWidth = windowWidth;
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
    const short h = menu->_textSize;
    Draw2D_Rect(x, y,
                  x + menu->_menuWidth,
                  y - h,
                  &menu->_highlight);

    Font_Print(menu->_font, &menu->_bg, x + 2, y, h, menu->_windowName);

    menu->_drawy -= h;
}

void _Menu_Borders()
{
    const short x = menu->_drawx;
    const short y = menu->_drawy;
    Draw2D_Rect(x, y,
                  x + menu->_menuWidth,
                  y - menu->_windowHeight,
                  &menu->_bg);
    Draw2D_RectLines(x, y,
                  x + menu->_menuWidth + 1,
                  y - menu->_windowHeight - 1,
                  &menu->_text);
}

void Menu_SetColors(Color4f* bg, Color4f* text, Color4f* highlight)
{
    if (menu == nullptr) return;

        menu->_bg = Color_CreateFromPointer4f(bg);
        menu->_text = Color_CreateFromPointer4f(text);
        menu->_highlight = Color_CreateFromPointer4f(highlight);
}

void Menu_Panel(int h, Color4f* color)
{
    if (menu == nullptr) return;

        const short x = menu->_drawx;
        const short y = menu->_drawy;
        const short w = menu->_menuWidth;
        Draw2D_Rect(x, y, x + w, y - h, color);
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
    const short h = menu->_textSize;
    Font_PrintAligned(menu->_font, &menu->_text, x, y, h, LJustify, LJustify, text);
    menu->_drawy -= h * menu->_rowHeightEm;
}

void Menu_Icon(IconSymbol icon, Color4f* color)
{

    if (menu == nullptr) return;
    const short x = menu->_drawx;
    const short y = menu->_drawy;
    const short h = menu->_textSize;
    Font_Icon(menu->_font, color, x, y, h, LJustify, LJustify, icon);
    menu->_drawy -= h * menu->_rowHeightEm;
}

bool Menu_Button(const char* text)
{
    if (menu == nullptr) return false;

    const short x = menu->_drawx;
    const short y = menu->_drawy;
    const short w = menu->_menuWidth;
    const short h = menu->_textSize * menu->_rowHeightEm;

    const short cx = V2f_X(menu->_cursorPosition);
    const short cy = V2f_Y(menu->_cursorPosition);

    bool inside = ((cx >= x) &&
                (cx <= x + w) &&
                (cy <= y) &&
                (cy >= y - h));

    Color4f* background = &menu->_bg;
    Color4f* pen = &menu->_text;
    if (inside)
    {
        background = &menu->_highlight;
        pen = &menu->_bg;
    }
    Draw2D_Rect(x, y, x + w, y - h, background);

    Font_PrintAligned(menu->_font, pen, x, y, menu->_textSize, LJustify, LJustify, text);
            
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
    const short h = menu->_textSize * menu->_rowHeightEm;
    const short cx = V2f_X(menu->_cursorPosition);
    const short cy = V2f_Y(menu->_cursorPosition);

    bool inside = ((cx >= x) &&
                (cx <= x + w) &&
                (cy <= y) &&
                (cy >= y - h));

    short padding = 2;

    Color4f* background = &menu->_bg;
    Color4f* pen = &menu->_text;
    if (inside)
    {
        background = &menu->_highlight;
        pen = &menu->_bg;
    }
    Draw2D_Rect(x, y, x + w, y - h, background);

    if (isOn)
    {
        Draw2D_Rect(x + padding, y - padding,
                      x + h - padding, y - h + padding, pen);
    }
    else
    {
        Draw2D_RectLines(x + padding, y - padding,
                      x + h - padding, y - h + padding, pen);
    }

    Font_PrintAligned(menu->_font, pen, x + h + padding * 2, y, menu->_textSize, LJustify, LJustify, text);

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
    const short h = menu->_textSize * menu->_rowHeightEm;

    Color4f* background = &menu->_bg;
    Color4f* pen = &menu->_text;
    if (enabled)
    {
        background = &menu->_highlight;
        pen = &menu->_bg;
    }
    Draw2D_Rect(x, y, x + w, y - h, background);

    Font_PrintAligned(menu->_font, pen, x+w/2, y, menu->_textSize, Centered, LJustify, text);

    menu->_drawy -= h ;
}


void Menu_Separator(void)
{
    // TODO
}

void Menu_Skip(short height)
{
    menu->_drawy -= height;
}

void Menu_DrawCursor(void)
{
    Font* db = DefaultFont_GetDefaultFont();
    Color4f* white = Color_GetDefaultColor(Color_White);
    short x = V2f_X(cursorPosition_);
    short y= V2f_Y(cursorPosition_);
    short w = db->characterWidth;
    short h = db->characterHeight;
    _Menu_DrawCursorParams(x+2, y-2, w, h, &menu->_bg);
    _Menu_DrawCursorParams(x, y, w, h, white);
}

void _Menu_DrawCursorParams(short x, short y, short w, short h, Color4f* color)
{

    Font* db = DefaultFont_GetDefaultFont();
    Font_Icon(db, color,
              x, y, h, LJustify, LJustify, Icon_CursorPoint);
    Font_Icon(db, color,
              x, y-h, h, LJustify, LJustify, Icon_CursorBase);
    Font_IconRotated(db, color,
              x + w, y-h+1, h, LJustify, LJustify, 1, Icon_CursorWing);

}
