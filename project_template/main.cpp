#include <mgdl-wii.h>
#include <wiiuse/wpad.h>
#include <string>

#include "mgdl-input-wii.h"

// Image
#include "konata_png.h"
gdl::Image konata;
// SpriteSet
#include "tilemap_png.h"
gdl::Image spriteSheet;
gdl::SpriteSet sprites;
// FFont
#include "amigaTopaz10_png.h"
gdl::Image topazBytefont;
gdl::FFont topaz;
#include "font8x16_png.h"
gdl::Image IBMFontImage;
gdl::FFont IBM8x16;
// Ogg
#include "sample3_ogg.h"
gdl::Music sampleMusic;
bool musicPlaying = false;
// Sound
#include "blipSelect_wav.h"
gdl::Sound blip;

gdl::MenuCreator menu(&topaz, 2.0f);

void init()
{
    fatInitDefault();
	gdl::InitSystem(gdl::ModeMPAL, gdl::Aspect16x9, gdl::HiRes);
    gdl::SetClearColor(gdl::Color::Black);
    gdl::WiiInput::Init();
    gdl::ConsoleMode();

    konata.LoadImageBuffer(konata_png, konata_png_size, gdl::Nearest, gdl::RGBA8);
    spriteSheet.LoadImageBuffer(tilemap_png, tilemap_png_size, gdl::Nearest, gdl::RGBA8);
    short spritesPerRow = 128/8;
    short rows = 80/8;
    // TODO: Calculate these automaticall from sprite size.
    gdl::SpriteSetConfig cfg = sprites.CreateConfig(spritesPerRow*rows, rows, spritesPerRow, 8,8);
    sprites.LoadSprites(cfg, &spriteSheet);

    topazBytefont.LoadImageBuffer(amigaTopaz10_png, amigaTopaz10_png_size, gdl::Nearest, gdl::RGBA8);
    topaz.BindSheet(topazBytefont, 9, 9, 32);

    IBMFontImage.LoadImageBuffer(font8x16_png, font8x16_png_size, gdl::Nearest, gdl::RGBA8);
    IBM8x16.BindSheet(IBMFontImage, 8, 16, 0);
    
    blip.LoadSound(blipSelect_wav, blipSelect_wav_size);
    sampleMusic.LoadFromBuffer(sample3_ogg, sample3_ogg_size);

    while(true)
    {
        // Show console for a while
        gdl::WiiInput::StartFrame();
        if (gdl::WiiInput::ButtonPress(WPAD_BUTTON_A))
        {
            break;
        }
    }
}

void DrawFont(short x, short y, gdl::FFont* font)
{
    float scale = 2.0f;
    font->DrawText(" !\"#$%&'()*+,-./", x, y, scale, gdl::Color::White);
    y+=font->GetHeight() * scale;
    font->DrawText("0123456789:;<=>?", x, y, scale, gdl::Color::White);
    y+=font->GetHeight() * scale;
    font->DrawText("@ABCDEFGHIJKLMNO", x, y, scale, gdl::Color::White);
    y+=font->GetHeight() * scale;
    font->DrawText("PQRSTUVWXYZ[\\]^_", x, y, scale, gdl::Color::White);
}

void DrawJoystick(short x, short y, short size)
{
    // Draw joystick direction
    short jsize=size;
    short box = jsize/3;
    short h=box/2;
    gdl::Color::ColorValues jc = gdl::Color::Green;
    gdl::vec2 jdir = gdl::WiiInput::GetNunchukJoystickDirection(0.0f);
    short jleft= x + jsize/2 + jdir.x * box-h;
    short jtop = y + jsize/2 + jdir.y * box-h;
    gdl::DrawBox(x, y, x+jsize, y+jsize, jc);
    if (jdir.x != 0.0f && jdir.y != 0.0f)
    {
        jc = gdl::Color::LightGreen;
    }
    gdl::DrawBoxF(jleft, jtop, jleft+box, jtop+box,jc);
}

void DrawDPad(short x, short y, short size)
{
    short box = size/3;
    short h=box/2;
    x += box + h;
    y += box + h;
    // Dpad
    int dpad_buttons[] = {
        WPAD_BUTTON_UP, 
        WPAD_BUTTON_DOWN,
        WPAD_BUTTON_LEFT,
        WPAD_BUTTON_RIGHT
    };
    gdl::vec2 directions[] = {
        gdl::vec2(0,-1), 
        gdl::vec2(0,1),
        gdl::vec2(-1,0),
        gdl::vec2(1,0)
    };
    for (int i=0;i<4;i++)
    {
        gdl::Color::ColorValues c = gdl::Color::LightRed;
        if (gdl::WiiInput::ButtonHeld(dpad_buttons[i]))
        {
            c = gdl::Color::Red;
        }
        gdl::vec2 d=directions[i];
        gdl::DrawBoxF(x+d.x*box-h, y+ d.y*box-h, x+box+d.x*box-h, y+box+d.y*box-h, c);
    }
}

void DrawButtons(short x, short y, short size, gdl::FFont* font)
{
    // Draw button states
    gdl::Color::ColorValues active = gdl::Color::Blue;
    gdl::Color::ColorValues inactive = gdl::Color::LightBlue;
    int buttons[] = {
        WPAD_BUTTON_A, 
        WPAD_BUTTON_B,
        WPAD_BUTTON_PLUS,
        WPAD_BUTTON_MINUS,
        WPAD_BUTTON_1,
        WPAD_BUTTON_2
        };
    static std::string names[] ={ "A", "B", "+", "-", "1", "2" };
    for(int i = 0; i < 6;i++ )
    {
        gdl::Color::ColorValues c = inactive;
        if (gdl::WiiInput::ButtonHeld(buttons[i]))
        {
            c = active;
        }
        gdl::DrawBoxF(x + i * size, y, x+size+i*size,y+size,c);
        font->DrawText(names[i].c_str(), x + i*size, y, 1.0f, gdl::Color::White);
    }
}

void DrawControllerDebug(short x, short y)
{
    // Draw cursor
    gdl::vec2 cp = gdl::WiiInput::GetCursorPosition();

    gdl::DrawLine(cp.x-10,cp.y,cp.x+10,cp.y,gdl::Color::White);
    gdl::DrawLine(cp.x,cp.y-10,cp.x,cp.y+10,gdl::Color::White);

    DrawButtons(x, y, 20, &IBM8x16);
    y += 20;
    DrawDPad(x, y, 60);
    x += 80;
    DrawJoystick(x, y, 60);
}

void DrawSprites(short x, short y, float scale)
{
    short tileSize = 8 * scale;
    short placeX = x;
    for (short i = 0; i < 160; i++)
    {
        if (i != 0 && i % 16 == 0)
        {
            placeX = x;
            y += tileSize;
        }
        sprites.Put(placeX, y, i, gdl::Color::White, gdl::Centered, gdl::Centered, scale);
        placeX += tileSize;
    }
}

void DrawMenu(short x, short y, short w)
{
    gdl::vec2 cp = gdl::WiiInput::GetCursorPosition();
    menu.StartMenu(x, y, w, cp.x, cp.y, gdl::WiiInput::ButtonPress(WPAD_BUTTON_A));
    if (menu.Button("Play Ogg", gdl::Color::LightBlue))
    {
        if (!musicPlaying)
        {
            sampleMusic.PlayMusic(true);
            musicPlaying = true;
        }
    }
    if (menu.Button("Play Sound", gdl::Color::LightBlue))
    {
        blip.Play(1.0f, 100.0f);
    }
    /*
    if (menu.Button("Assert"))
    {
        gdl_assert(false, "Assert fired!");
    }
    */
    menu.Text("Hi! I am menu.");
    menu.Text("Yellow panel");
    menu.Panel(40, gdl::Color::Yellow);
}

void DrawTiming(short x, short y, gdl::FFont* font)
{
    font->Printf(x, y, 2, gdl::Color::White, "Deltatime: %f", gdl::Delta);
    // TODO: Add gdl::Elapsed;
    font->Printf(x, y + 30, 2, gdl::Color::White, "Elapsed time: %f", sampleMusic.GetElapsed());
}

int main()
{
    init();
    menu = gdl::MenuCreator(&IBM8x16, 1.0f);
    float boxX = 0;
    float elapsed = 0.0f;
    short frameCount = 0;
    while(1)
    {
        gdl::WiiInput::StartFrame();

        if (gdl::WiiInput::ButtonPress(WPAD_BUTTON_HOME)){
            gdl::wii::Exit();
        }

        if (gdl::WiiInput::ButtonPress(WPAD_BUTTON_B)){
            gdl::wii::Exit();
        }

        gdl::PrepDisplay();
        // Input
        short top = 40;
        short left = 40;
        DrawControllerDebug(left, top);
        left += 160;
        DrawMenu(left, top, 120);
        left += 140;
        DrawTiming(left, top, &IBM8x16);

        // SECOND ROW//////////////////////////////////////////////////////
        left = 40;
        top = gdl::ScreenCenterY - 40;

        // Draw Image
        IBM8x16.DrawText("Gambatte minnasan!", 10, top - 32, 2.0f, gdl::Color::White);
        konata.Put(left, top, gdl::Color::White, 0, 0, 0.4f);
        left += konata.Xsize() * 0.4f + 10;

        // Draw Sprites
        DrawSprites(left, top, 1.0f);
        left += 16*8;

        DrawFont(left, top, &IBM8x16);

        top += IBM8x16.GetHeight() * 2 * 6;
        DrawFont(left, top, &topaz);

        // Draw Moving square
        gdl::DrawBoxF(boxX, gdl::ScreenYres-100, boxX+60, gdl::ScreenYres-60, gdl::Color::LightGreen);
        gdl::Display();

        boxX += gdl::Delta;
        if (boxX > gdl::ScreenXres)
        {
            boxX = 0;
        }
        frameCount++;
    }
}