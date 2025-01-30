#include <mgdl/mgdl-gui.h>
#include <mgdl/mgdl-font.h>
#include <mgdl/mgdl-draw2d.h>

gdl::MenuCreator::MenuCreator()
{
    currentFont = nullptr;
    textHeight = 1.0f;
}

gdl::MenuCreator::MenuCreator(gdl::Font* font, float textHeight, float rowHeightEm)
{
    this->currentFont = font;
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
    gdl::DrawBoxF(x, y, x + w, y - h, color);
    y -= h;
}

void gdl::MenuCreator::Text(const char* text)
{
    int h = currentFont->GetCharacterHeight() * textHeight * rowHeightEm;
    currentFont->Print(this->text, x, y, h/rowHeightEm, LJustify, LJustify, text);
    y -= h;
}

bool gdl::MenuCreator::Button(const char* text)
{
    int h = currentFont->GetCharacterHeight() * textHeight * rowHeightEm;

    bool inside = ((cursorX >= x) &&
                (cursorX <= x + w) &&
                (cursorY <= y) &&
                (cursorY >= y - h));

    if (inside)
    {
        gdl::DrawBoxF(x, y, x + w, y - h, highlight);
    }
    else
    {
        gdl::DrawBoxF(x, y, x + w, y - h, bg);
        gdl::DrawBox(x, y, x + w, y - h, border);
    }

    // TODO Center text

    float textH = h/rowHeightEm;
    currentFont->Print(this->text, x, y, textH, LJustify, LJustify, text);
            
    y -= h ;

    return (inside && buttonPress);
}

bool gdl::MenuCreator::Toggle ( const char* text, bool& valueRef )
{
    int h = currentFont->GetCharacterHeight() * textHeight * rowHeightEm;

    bool inside = ((cursorX >= x) &&
                (cursorX <= x + w) &&
                (cursorY <= y) &&
                (cursorY >= y - h));

    short padding = 2;
    if (inside)
    {
        gdl::DrawBoxF(x, y, x + w, y - h, highlight);
    }
    else
    {
        gdl::DrawBoxF(x, y, x + w, y - h, bg);
        gdl::DrawBox(x, y, x + w, y - h, border);
    }

    if (valueRef)
    {
        gdl::DrawBoxF(x + padding, y - padding, x + h - padding, y - h + padding, this->text);
    }
    else
    {
        gdl::DrawBox(x + padding, y - padding, x + h - padding, y - h + padding, this->text);
    }

    float textH = h/rowHeightEm;
    currentFont->Print(this->text, x + h + padding, y, textH, LJustify, LJustify, text);

    y -= h ;

    if (inside && buttonPress)
    {
        valueRef = !valueRef;
    }

    return (inside && buttonPress);
}

