#include "mgdl-wii/mgdl-gui.h"
#include "mgdl-wii/mgdl-basic.h"
#include "mgdl-wii/mgdl-font.h"
#include <wiiuse/wpad.h>

gdl::MenuCreator::MenuCreator()
{
    currentFont = nullptr;
    fontScale = 1.0f;
}

gdl::MenuCreator::MenuCreator(gdl::FFont* font, float fontScale, float rowHeightEm)
{
    this->currentFont = font;
    this->fontScale = fontScale;
    this->rowHeightEm = rowHeightEm;

    // Default colors    
    this->bg = gdl::Color::Black;
    this->border = gdl::Color::Blue;
    this->text = gdl::Color::White;
    this->highlight = gdl::Color::LightBlue;
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

void gdl::MenuCreator::SetColors(gdl::Color::ColorValues bg, gdl::Color::ColorValues border, gdl::Color::ColorValues text, gdl::Color::ColorValues highlight)
{
    this->bg = bg;
    this->border = border;
    this->text = text; 
    this->highlight = highlight;
}

void gdl::MenuCreator::Panel(int h, u_int color)
{
    gdl::DrawBoxF(x, y, x + w, y + h, color);
    y += h;
}

void gdl::MenuCreator::Text(const char* text)
{
    int h = currentFont->GetHeight() * fontScale * rowHeightEm;
    currentFont->DrawText(text, x, y, fontScale, this->text);
    y += h;
}

bool gdl::MenuCreator::Button(const char* text)
{
    int h = currentFont->GetHeight() * fontScale * rowHeightEm;

    bool inside = ((cursorX >= x) &&
                (cursorX <= x + w) &&
                (cursorY >= y) &&
                (cursorY <= y + h));

    gdl::DrawBoxF(x, y, x + w, y + h, bg);
    if (inside)
    {
        gdl::DrawBoxF(x, y, x + w, y + h, highlight);
    }
    else
    {
        gdl::DrawBox(x, y, x + w, y + h, border);
    }

    // TODO Center text
    currentFont->DrawText(text, x, y, fontScale, this->text);
            
    y += h ;

    return (inside && buttonPress);
}
