#include <mgdl/mgdl-gui.h>
#include <mgdl/mgdl-font.h>
#include <mgdl/mgdl-draw2d.h>

gdl::MenuCreator::MenuCreator()
{
    currentFont = nullptr;
    fontScale = 1.0f;
}

gdl::MenuCreator::MenuCreator(gdl::Font* font, float fontScale, float rowHeightEm)
{
    this->currentFont = font;
    this->fontScale = fontScale;
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
    gdl::DrawBoxF(x, y, x + w, y + h, color);
    y += h;
}

void gdl::TranslateTo(short x, short y)
{
	glTranslatef((float)x, (float)y, 0.0f);
}

void gdl::MenuCreator::Text(const char* text)
{
    int h = currentFont->GetCharacterHeight() * fontScale * rowHeightEm;
    TranslateTo(x, y);
    currentFont->Print(this->text, fontScale, LJustify, LJustify, text);
    TranslateTo(-x, -y);
    y += h;
}

bool gdl::MenuCreator::Button(const char* text)
{
    int h = currentFont->GetCharacterHeight() * fontScale * rowHeightEm;

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

    TranslateTo(x, y);
    currentFont->Print(this->text, fontScale, LJustify, LJustify, text);
    TranslateTo(-x, -y);
            
    y += h ;

    return (inside && buttonPress);
}
