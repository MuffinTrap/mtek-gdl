#include <mgdl/mgdl-gui.h>
#include <mgdl/mgdl-font.h>
#include <mgdl/mgdl-defaultfont.h>
#include <mgdl/mgdl-draw2d.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-controller.h>
#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-palette.h>
#include <mgdl/mgdl-main.h>


void Menu_ReadDefaultInputs(Menu* menu)
{
    WiiController* c = Platform_GetController(0);
    menu->cursorPosition = WiiController_GetCursorPosition(c);
    menu->buttonHeld = WiiController_ButtonHeld(c, ButtonA);
    menu->buttonPress = WiiController_ButtonPress(c, ButtonA);
}


Menu* Menu_CreateDefault()
{
    return Menu_Create(DefaultFont_GetDefaultFont(), 1.0f, 1.1f);
}

Menu* Menu_Create(Font* font, float textHeight, float rowHeightEm)
{
    Menu* menu = (Menu*)malloc(sizeof(Menu));

    menu->font = font;
    menu->textHeight = textHeight;
    menu->rowHeightEm = rowHeightEm;
    menu->textSize = textHeight * font->characterHeight;
    menu->drawWindow = false;

    // TODO Calculate text and row heights just once

    // Default colors    //TODO Change to palette colors
    Palette* blessing = Palette_GetDefault();

    menu->bg = Palette_GetColor4f(blessing, 1);
    menu->text = Palette_GetColor4f(blessing, 5);
    menu->highlight = Palette_GetColor4f(blessing, 3);

    menu->drawDirection = MenuDownward;
    menu->largestHeightOnRow = 0.0f;

    return menu;
}

Menu* Menu_CreateWindowed(Font* font, float textHeight, float rowHeightEm, short windowWidth, short windowHeight, const char* title)
{
    Menu* menu = Menu_Create(font, textHeight, rowHeightEm);
    menu->drawWindow = true;
    menu->windowName = title;
    menu->menuWidth = windowWidth;
    menu->windowHeight = windowHeight;
    // Invalid starting values
    menu->windowx = -1;
    menu->windowy = -1;

    return menu;
}


void Menu_Start(Menu* menu, short x, short y, short width)
{
    WiiController* c = Platform_GetController(0);
    Menu_StartInput(menu, x, y, width,
        WiiController_GetCursorPosition(c),
        WiiController_ButtonPress(c, ButtonA),
        WiiController_ButtonHeld(c, ButtonA));

}

void Menu_StartInput(Menu* menu, short x, short y, short width, vec2 cursorPosition, bool buttonPress, bool buttonHeld)
{
    if (menu != nullptr)
    {
        if (menu->drawWindow)
        {
            if( menu->windowx < 0)
            {
                menu->windowx = x;
                menu->windowy = y;
            }
            menu->drawx = menu->windowx;
            menu->drawy = menu->windowy;
            Menu_Borders_(menu);
            Menu_TitleBar_(menu);
        }
        else
        {
            menu->drawx = x;
            menu->drawy = y;
            menu->menuWidth = width;
        }
        menu->cursorPosition = cursorPosition;
        menu->buttonPress = buttonPress;
        menu->buttonHeld = buttonHeld;

        menu->largestHeightOnRow = 0.0f;
        menu->startx = menu->drawx;
    }

}

void Menu_BeginRow(Menu* menu)
{
    menu->largestHeightOnRow = 0.0f;
    menu->drawDirection = MenuRightward;
}
void Menu_EndRow(Menu* menu)
{
    menu->drawDirection = MenuDownward;
    menu->drawx = menu->startx;
    menu->drawy -= menu->largestHeightOnRow;
    menu->largestHeightOnRow = 0.0f;
}

void Menu_TitleBar_(Menu* menu)
{
    const short x = menu->drawx;
    const short y = menu->drawy;
    const short h = menu->textSize;
    const short w = menu->menuWidth;
    Draw2D_Rect(x, y,
                  x + w,
                  y - h,
                  &menu->highlight);

    Font_Print(menu->font, &menu->bg, x + 2, y, h, menu->windowName);

    // NOTE titlebar cannot be on a row
    menu->drawy -= h;
}

void Menu_Borders_(Menu* menu)
{
    const short x = menu->drawx;
    const short y = menu->drawy;
    Draw2D_Rect(x, y,
                  x + menu->menuWidth,
                  y - menu->windowHeight,
                  &menu->bg);
    Draw2D_RectLines(x, y,
                  x + menu->menuWidth + 1,
                  y - menu->windowHeight - 1,
                  &menu->text);
}

void Menu_SetColors(Menu* menu, Color4f* bg, Color4f* text, Color4f* highlight)
{
    if (menu == nullptr) return;

        menu->bg = Color_CreateFromPointer4f(bg);
        menu->text = Color_CreateFromPointer4f(text);
        menu->highlight = Color_CreateFromPointer4f(highlight);
}

void Menu_TextF(Menu* menu, const char* text, ...)
{
    MGDL_PRINTF_TO_BUFFER(text);
    Menu_Text(menu, mgdl_GetPrintfBuffer());
}

void Menu_Text(Menu* menu, const char* text)
{
    if (menu == nullptr) return;

    const short x = menu->drawx;
    const short y = menu->drawy;
    const short h = menu->textSize;
    Font_PrintAligned(menu->font, &menu->text, x, y, h, LJustify, LJustify, text);

    float drawh = h * menu->rowHeightEm;
    menu->largestHeightOnRow = maxF(menu->largestHeightOnRow, drawh);

    switch(menu->drawDirection)
    {
        case MenuDownward: menu->drawy -= drawh; break;
        case MenuRightward: menu->drawx += strlen(text) * menu->font->characterWidth; break;
    }
}

void Menu_Icon(Menu* menu, IconSymbol icon, Color4f* color)
{

    if (menu == nullptr) return;
    const short x = menu->drawx;
    const short y = menu->drawy;
    const short h = menu->textSize;
    Font_Icon(menu->font, color, x, y, h, LJustify, LJustify, icon);

    float drawh = h * menu->rowHeightEm;
    menu->largestHeightOnRow = maxF(menu->largestHeightOnRow, drawh);
    switch(menu->drawDirection)
    {
        case MenuDownward: menu->drawy -= drawh; break;
        case MenuRightward: menu->drawx += menu->font->characterWidth; break;
    }
}

bool Menu_Button(Menu* menu, const char* text)
{
    if (menu == nullptr) return false;

    const short x = menu->drawx;
    const short y = menu->drawy;
    const short w = menu->menuWidth;
    const short h = menu->textSize * menu->rowHeightEm;

    const short cx = V2f_X(menu->cursorPosition);
    const short cy = V2f_Y(menu->cursorPosition);

    bool inside = ((cx >= x) &&
                (cx <= x + w) &&
                (cy <= y) &&
                (cy >= y - h));

    Color4f* background = &menu->bg;
    Color4f* pen = &menu->text;
    if (inside)
    {
        background = &menu->highlight;
        pen = &menu->bg;
    }
    Draw2D_Rect(x, y, x + w, y - h, background);

    Font_PrintAligned(menu->font, pen, x, y, menu->textSize, LJustify, LJustify, text);

    float drawh = h * menu->rowHeightEm;
    menu->largestHeightOnRow = maxF(menu->largestHeightOnRow, drawh);
    switch(menu->drawDirection)
    {
        case MenuDownward: menu->drawy -= h; break;
        case MenuRightward: menu->drawx += w; break;
    }

    return (inside && menu->buttonPress);
}

bool Menu_Slider(Menu* menu, const char* text, float minValue, float maxValue, float* valueRef)
{
    if (menu == nullptr) return false;

    const short x = menu->drawx;
    const short y = menu->drawy;
    const short w = menu->menuWidth;
    const short h = menu->textSize * menu->rowHeightEm;

    const short cx = V2f_X(menu->cursorPosition);
    const short cy = V2f_Y(menu->cursorPosition);

    bool inside = ((cx >= x) &&
                (cx <= x + w) &&
                (cy <= y) &&
                (cy >= y - h));

    Color4f* background = &menu->bg;
    Color4f* pen = &menu->text;
    Color4f* bar = &menu->highlight;
    if (inside)
    {
        background = &menu->highlight;
        bar = &menu->bg;
        pen = &menu->text;
    }
    Draw2D_RectLines(x, y, x + w, y - h, background);
    float range = maxValue-minValue; // 100 - (-100) -> 200
    float fill = ((*valueRef)-minValue)/range;
    // value - min
    // 0-(-100) -> 100 /200 -> 0.5f
    // 50 --100 -> 150 / 200  -> 0.75f
    // -100 --100 -> 0 / 200 -> 0.0f
    Draw2D_Rect(x, y, x + w * fill , y - h, bar);

    Font_PrintfAligned(menu->font, pen, x, y, menu->textSize, LJustify, LJustify, "%s:%.1f", text, *valueRef);

    float drawh = h * menu->rowHeightEm;
    menu->largestHeightOnRow = maxF(menu->largestHeightOnRow, drawh);
    switch(menu->drawDirection)
    {
        case MenuDownward: menu->drawy -= h; break;
        case MenuRightward: menu->drawx += w; break;
    }
    if (inside && menu->buttonPress)
    {
        float into_x = cx - x;
        float fill_x = into_x / w;
        float new_value = minValue + (maxValue-minValue) * fill_x;
        *valueRef = new_value;
        return true;
    }

    return false;

}

bool Menu_TexturedButton(Menu* menu, Texture* texture, TextureFlipModes flipflags)
{
    if (menu == nullptr) return false;

    const short x = menu->drawx;
    const short y = menu->drawy;
    const short w = menu->menuWidth;
    const short h = w / texture->aspectRatio;

    const short cx = V2f_X(menu->cursorPosition);
    const short cy = V2f_Y(menu->cursorPosition);

    bool inside = ((cx >= x) &&
                (cx <= x + w) &&
                (cy <= y) &&
                (cy >= y - h));

    Texture_SetTint(texture, 1.0f, 1.0f, 1.0f);
    if (!inside)
    {
        Texture_SetTint(texture, 0.7f, 0.7f, 0.7f);
    }
    short tx1 = x;
    short tx2 = x+w;
    short ty1 = y;
    short ty2 = y - h;

    if (Flag_IsSet(flipflags, FlipVertical) || Flag_IsSet(flipflags, FlipHorizontal))
    {
        glDisable(GL_CULL_FACE);
    }
    if (Flag_IsSet(flipflags, FlipVertical))
    {
            tx1 = x+w;
            tx2 = x;
    }
    if (Flag_IsSet(flipflags, FlipHorizontal))
    {
            ty1 = y-h;
            ty2 = y;
    }

    Texture_Draw2DAbsolute(texture, tx1, ty1, tx2, ty2);

    if (Flag_IsSet(flipflags, FlipVertical) || Flag_IsSet(flipflags, FlipHorizontal))
    {
        glEnable(GL_CULL_FACE);
    }

    Texture_SetTint(texture, 1.0f, 1.0f, 1.0f);

    menu->largestHeightOnRow = maxF(menu->largestHeightOnRow, h);
    switch(menu->drawDirection)
    {
        case MenuDownward: menu->drawy -= h; break;
        case MenuRightward: menu->drawx += w; break;
    }

    return (inside && menu->buttonPress);
}

bool Menu_IsCursorInside(Menu* menu, short w, short h)
{
    const short x = menu->drawx;
    const short y = menu->drawy;

    const short cx = V2f_X(menu->cursorPosition);
    const short cy = V2f_Y(menu->cursorPosition);

    bool inside = ((cx >= x) &&
                (cx <= x + w) &&
                (cy <= y) &&
                (cy >= y - h));

    return inside;
}

bool Menu_Toggle (Menu* menu,const char* text, bool* valuePtr )
{
    if (menu == nullptr) return false;

    bool isOn = *valuePtr;

    const short x = menu->drawx;
    const short y = menu->drawy;
    const short w = menu->menuWidth;
    const short h = menu->textSize * menu->rowHeightEm;
    const short cx = V2f_X(menu->cursorPosition);
    const short cy = V2f_Y(menu->cursorPosition);

    bool inside = ((cx >= x) &&
                (cx <= x + w) &&
                (cy <= y) &&
                (cy >= y - h));

    short padding = 2;

    Color4f* background = &menu->bg;
    Color4f* pen = &menu->text;
    if (inside)
    {
        background = &menu->highlight;
        pen = &menu->bg;
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

    Font_PrintAligned(menu->font, pen, x + h + padding * 2, y, menu->textSize, LJustify, LJustify, text);

    float drawh = h + 1;

    menu->largestHeightOnRow = maxF(menu->largestHeightOnRow, drawh);
    switch(menu->drawDirection)
    {
        case MenuDownward:  menu->drawy -= drawh; break;
        case MenuRightward: menu->drawx += w;
    };

    if (inside && menu->buttonPress)
    {
        *valuePtr = (!isOn);
    }

    return (inside && menu->buttonPress);
}

void Menu_Flag(Menu* menu, const char* text, bool enabled)
{
    const short x = menu->drawx;
    const short y = menu->drawy;
    const short w = menu->menuWidth;
    const short h = menu->textSize * menu->rowHeightEm;

    Color4f* background = &menu->bg;
    Color4f* pen = &menu->text;
    if (enabled)
    {
        background = &menu->highlight;
        pen = &menu->bg;
    }
    Draw2D_Rect(x, y, x + w, y - h, background);

    Font_PrintAligned(menu->font, pen, x+w/2, y, menu->textSize, Centered, LJustify, text);

    menu->largestHeightOnRow = maxF(menu->largestHeightOnRow, h);
    switch(menu->drawDirection)
    {
        case MenuDownward:  menu->drawy -= h; break;
        case MenuRightward: menu->drawx += w; break;
    };
}



void Menu_Skip(Menu* menu, short pixels)
{
    menu->largestHeightOnRow = maxF(menu->largestHeightOnRow, pixels);
    switch(menu->drawDirection)
    {
        case MenuDownward:  menu->drawy -= pixels; break;
        case MenuRightward: menu->drawx += pixels; break;
    };
}

void Menu_DrawCursor(Menu* menu)
{
    Font* db = DefaultFont_GetDefaultFont();
    Color4f* white = Color_GetDefaultColor(Color_White);
    short x = V2f_X(menu->cursorPosition);
    short y = V2f_Y(menu->cursorPosition);
    short w = db->characterWidth;
    short h = db->characterHeight;
    Menu_DrawCursorParams_(x+2, y-2, w, h, Color_GetDefaultColor(Color_Black));
    Menu_DrawCursorParams_(x, y, w, h, white);
}

void Menu_DrawCursorParams_(short x, short y, short w, short h, Color4f* color)
{

    Font* db = DefaultFont_GetDefaultFont();
    Font_Icon(db, color,
              x, y, h, LJustify, LJustify, Icon_CursorPoint);
    Font_Icon(db, color,
              x, y-h, h, LJustify, LJustify, Icon_CursorBase);
    Font_IconRotated(db, color,
              x + w, y-h+1, h, LJustify, LJustify, 1, Icon_CursorWing);

}
