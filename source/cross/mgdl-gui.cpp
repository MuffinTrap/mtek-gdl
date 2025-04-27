#include <mgdl/mgdl-gui.h>
#include <mgdl/mgdl-font.h>
#include <mgdl/mgdl-debugfont.h>
#include <mgdl/mgdl-draw2d.h>
#include <mgdl/mgdl-util.h>

gdl::MenuCreator::MenuCreator()
{
    font = GetDebugFont();
    textHeight = 1.0f;
    rowHeightEm = 1.1f;

    this->bg = gdl::Colors::Black;
    this->border = gdl::Colors::Blue;
    this->text = gdl::Colors::White;
    this->highlight = gdl::Colors::LightBlue;
}

gdl::MenuCreator::MenuCreator(Font* font, float textHeight, float rowHeightEm)
{
    this->font = font;
    this->textHeight = textHeight;
    this->rowHeightEm = rowHeightEm;

    // Default colors    
    this->bg = gdl::Colors::Black;
    this->border = gdl::Colors::Blue;
    this->text = gdl::Colors::White;
    this->highlight = gdl::Colors::LightBlue;
}

void gdl::MenuCreator::StartMenu(int x, int y, int w, float cursorX, float cursorY, bool buttonPress)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->cursorX = cursorX;
    this->cursorY = cursorY;
    this->buttonPress = buttonPress;
}

void gdl::MenuCreator::SetColors(u32 bg, u32 border, u32 text, u32 highlight)
{
    this->bg = bg;
    this->border = border;
    this->text = text; 
    this->highlight = highlight;
}

void gdl::MenuCreator::Panel(int h, u32 color)
{
    DrawBoxF(x, y, x + w, y - h, color);
    y -= h;
}
void gdl::MenuCreator::TextF(const char* text, ...)
{
    MGDL_PRINTF_TO_BUFFER(text);
    Text(MGDL_GetPrintfBuffer());
}

void gdl::MenuCreator::Text(const char* text)
{
    int h = font->ch * textHeight * rowHeightEm;
    Font_PrintAligned(font, this->text, x, y, h/rowHeightEm, LJustify, LJustify, text);
    y -= h;
}

bool gdl::MenuCreator::Button(const char* text)
{
    int h = font->ch * textHeight * rowHeightEm;

    bool inside = ((cursorX >= x) &&
                (cursorX <= x + w) &&
                (cursorY <= y) &&
                (cursorY >= y - h));

    if (inside)
    {
        DrawBoxF(x, y, x + w, y - h, highlight);
    }
    else
    {
        DrawBoxF(x, y, x + w, y - h, bg);
        DrawBox(x, y, x + w, y - h, border);
    }

    // TODO Center text

    float textH = h/rowHeightEm;
    Font_PrintAligned(font, this->text, x, y, textH, LJustify, LJustify, text);
            
    y -= h ;

    return (inside && buttonPress);
}

bool gdl::MenuCreator::Toggle ( const char* text, bool& valueRef )
{
    int h = font->ch * textHeight * rowHeightEm;

    bool inside = ((cursorX >= x) &&
                (cursorX <= x + w) &&
                (cursorY <= y) &&
                (cursorY >= y - h));

    short padding = 2;
    if (inside)
    {
        DrawBoxF(x, y, x + w, y - h, highlight);
    }
    else
    {
        DrawBoxF(x, y, x + w, y - h, bg);
        DrawBox(x, y, x + w, y - h, border);
    }

    if (valueRef)
    {
        DrawBoxF(x + padding, y - padding, x + h - padding, y - h + padding, this->text);
    }
    else
    {
        DrawBox(x + padding, y - padding, x + h - padding, y - h + padding, this->text);
    }

    float textH = h/rowHeightEm;
    Font_PrintAligned(font, this->text, x + h + padding, y, textH, LJustify, LJustify, text);

    y -= h ;

    if (inside && buttonPress)
    {
        valueRef = !valueRef;
    }

    return (inside && buttonPress);
}

