#include <mgdl/mgdl-gui.h>
#include <mgdl/mgdl-texture.h>
#include <mgdl/mgdl-spriteatlas.h>
#include <mgdl/mgdl-defaultfont.h>
#include <mgdl/mgdl-script-api.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-controller.h>
#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-palette.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-memory.h>


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

Menu* Menu_Create(Texture* font, float textHeight, float rowHeightEm)
{
    Menu* menu = (Menu*)mgdl_AllocateGeneralMemory(sizeof(Menu));

    menu->font = font;
    menu->textHeight = textHeight;
    menu->rowHeightEm = rowHeightEm;
    menu->textSize = textHeight * font->spriteAtlas->characterHeight;
    menu->drawWindow = false;

    // TODO Calculate text and row heights just once

    // Default colors    //TODO Change to palette colors
    Palette* blessing = Palette_GetDefault();

    menu->bg = Palette_GetColor(blessing, 1);
    menu->text = Palette_GetColor(blessing, 5);
    menu->highlight = Palette_GetColor(blessing, 3);

    menu->drawDirection = MenuDownward;
    menu->largestHeightOnRow = 0.0f;

    return menu;
}

Menu* Menu_CreateWindowed(Texture* font, float textHeight, float rowHeightEm, short windowWidth, short windowHeight, const char* title)
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

void Menu_StartInput(Menu* menu, short x, short y, short width, Vector2 cursorPosition, bool buttonPress, bool buttonHeld)
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
    mgdl_DrawRectangle(x, y,
                  x + w,
                  y - h,
                  menu->highlight);

    Texture_DrawText(menu->font, menu->bg, x + 2, y, h, menu->windowName);

    // NOTE titlebar cannot be on a row
    menu->drawy -= h;
}

void Menu_Borders_(Menu* menu)
{
    const short x = menu->drawx;
    const short y = menu->drawy;
    mgdl_DrawRectangle(x, y,
                  x + menu->menuWidth,
                  y - menu->windowHeight,
                  menu->bg);
    mgdl_DrawRectangleLines(x, y,
                  x + menu->menuWidth + 1,
                  y - menu->windowHeight - 1,
                  menu->text);
}

void Menu_SetColors(Menu* menu, color32 bg, color32 text, color32 highlight)
{
    if (menu == nullptr) return;

        menu->bg = (bg);
        menu->text = (text);
        menu->highlight = (highlight);
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
    Texture_DrawText(menu->font, menu->text, x, y, h, text);

    float drawh = h * menu->rowHeightEm;
    menu->largestHeightOnRow = maxF(menu->largestHeightOnRow, drawh);

    switch(menu->drawDirection)
    {
        case MenuDownward: menu->drawy -= drawh; break;
        case MenuRightward: menu->drawx += strlen(text) * menu->font->spriteAtlas->characterWidth; break;
    }
}

void Menu_Icon(Menu* menu, IconSymbol icon, color32 color)
{

    if (menu == nullptr) return;
    const short x = menu->drawx;
    const short y = menu->drawy;
    const short h = menu->textSize;
    DefaultFont_DrawIcon(color, x, y, h, icon);

    float drawh = h * menu->rowHeightEm;
    menu->largestHeightOnRow = maxF(menu->largestHeightOnRow, drawh);
    switch(menu->drawDirection)
    {
        case MenuDownward: menu->drawy -= drawh; break;
        case MenuRightward: menu->drawx += menu->font->spriteAtlas->characterWidth; break;
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

    color32 background = menu->bg;
    color32 pen = menu->text;
    if (inside)
    {
        pen = menu->highlight;
    }
    mgdl_DrawRectangle(x, y, x + w, y - h, background);

    Texture_DrawText(menu->font, pen, x, y, menu->textSize, text);

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

    color32 background = menu->bg;
    color32 pen = menu->text;
    color32 bar = menu->highlight;
    if (inside)
    {
        background = menu->highlight;
        bar = menu->bg;
        pen = menu->text;
    }
    mgdl_DrawRectangleLines(x, y, x + w, y - h, background);
    float range = maxValue-minValue; // 100 - (-100) -> 200
    float fill = ((*valueRef)-minValue)/range;
    // value - min
    // 0-(-100) -> 100 /200 -> 0.5f
    // 50 --100 -> 150 / 200  -> 0.75f
    // -100 --100 -> 0 / 200 -> 0.0f
    mgdl_DrawRectangle(x, y, x + w * fill , y - h, bar);

    Texture_DrawTextF(menu->font, pen, x, y, menu->textSize, "%s:%.4f", text, *valueRef);

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

    color32 background = menu->bg;
    color32 pen = menu->text;
    if (inside)
    {
        background = menu->highlight;
        pen = menu->bg;
    }
    mgdl_DrawRectangle(x, y, x + w, y - h, background);

    if (isOn)
    {
        mgdl_DrawRectangle(x + padding, y - padding,
                      x + h - padding, y - h + padding, pen);
    }
    else
    {
        mgdl_DrawRectangleLines(x + padding, y - padding,
                      x + h - padding, y - h + padding, pen);
    }

    Texture_DrawText(menu->font, pen, x + h + padding * 2, y, menu->textSize, text);

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

    color32 background = menu->bg;
    color32 pen = menu->text;
    if (enabled)
    {
        background = menu->highlight;
        pen = menu->bg;
    }
    mgdl_DrawRectangle(x, y, x + w, y - h, background);

    Vector2 pos = mgdl_CalculateAlignedTopLeft(x+w/2, y, strlen(text) * menu->font->spriteAtlas->characterWidth ,menu->textSize, Centered, LJustify);

    Texture_DrawText(menu->font, pen, pos.x, pos.y, menu->textSize, text);

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
    Texture* db = DefaultFont_GetDefaultFont();
    color32 white = Debug_White;
    short x = V2f_X(menu->cursorPosition);
    short y = V2f_Y(menu->cursorPosition);
    short w = db->spriteAtlas->characterWidth;
    short h = db->spriteAtlas->characterHeight;
    Menu_DrawCursorParams_(x+2, y-2, w, h, Debug_Black);
    Menu_DrawCursorParams_(x, y, w, h, white);
}

void Menu_DrawCursorParams_(short x, short y, short w, short h, color32 color)
{

    DefaultFont_DrawIcon( color,
              x, y, h, Icon_CursorPoint);
    DefaultFont_DrawIcon( color,
              x, y-h, h, Icon_CursorBase);
    DefaultFont_DrawIconRotated( color,
              x + w, y-h+1, h, 1, Icon_CursorWing);

}
