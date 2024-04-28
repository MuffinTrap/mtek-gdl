#include <mgdl-wii.h>
#include <wiiuse/wpad.h>
#include <string>

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
// Ogg
#include "sample3_ogg.h"
gdl::Music sampleMusic;
bool musicPlaying = false;
// Sound
#include "blipSelect_wav.h"
gdl::Sound blip;

static gdl::Font defaultFont;


// For reasons beyond my understading
// the input reading cannot be in a 
// classless function...???
static gdl::WiiInput input;
// gdl::MenuCreator menu(topaz, 8);


void init()
{
    fatInitDefault();
	gdl::InitSystem(gdl::ModeMPAL, gdl::Aspect16x9, gdl::HiRes);
    gdl::SetClearColor(gdl::Color::Black);
    // input.Init();
    WPAD_Init();
    char buffer[100];
    // sprintf(buffer, "Wpad init flag: %d", (s32)input.GetInitStatus());
    // gdl_assert((input.GetInitStatus() == gdl::WiiInputStatus::AllOk), buffer);
    gdl::ConsoleMode();

    konata.LoadImageBuffer(konata_png, konata_png_size, gdl::Nearest, gdl::RGBA8);
    spriteSheet.LoadImageBuffer(tilemap_png, tilemap_png_size, gdl::Nearest, gdl::RGBA8);
    short spritesPerRow = 128/8;
    short rows = 80/8;
    // TODO: Calculate these automaticall from sprite size.
    gdl::SpriteSetConfig cfg = sprites.CreateConfig(spritesPerRow*rows, rows, spritesPerRow, 8,8);
    sprites.LoadSprites(cfg, &spriteSheet);

    topazBytefont.LoadImageBuffer(amigaTopaz10_png, amigaTopaz10_png_size, gdl::Nearest, gdl::RGBA8);
    // Space is 32 in Ascii
    // DEL is 127
    // LastIndex is exclusive
    // TODO: add parameter for gap between characters
    // TODO: Allow other widths than 16 characters
    topaz.BindSheet(topazBytefont, 8, 8, 32, 128);
    
    // blip.LoadSoundBuffer(blib_wav, blib_wav_size);

    defaultFont.LoadFontMem(gdl::DefaultFontData);
    sampleMusic.LoadFromBuffer(sample3_ogg, sample3_ogg_size);
}

// TODO: Maybe remove the enum?
void PrintStatus(gdl::WiiInputStatus s, short x, short y)
{
    switch(s)
    {
        case gdl::WiiInputStatus::AllOk:
            defaultFont.DrawText("Input OK", x, y, 2, gdl::Color::White);
            break;
        case gdl::WiiInputStatus::NotReady:
            defaultFont.DrawText("Input not ready", x, y, 2, gdl::Color::LightBlue);
            break;
        case gdl::WiiInputStatus::NoController:
            defaultFont.DrawText("No Controller", x, y, 2, gdl::Color::LightRed);
            break;
        case gdl::WiiInputStatus::TransferError:
            defaultFont.DrawText("Transfer error", x, y, 2, gdl::Color::LightRed);
            break;
        case gdl::WiiInputStatus::NoneRegistered:
            defaultFont.DrawText("None registered", x, y, 2, gdl::Color::LightRed);
            break;
        case gdl::WiiInputStatus::UnknownError:
            defaultFont.DrawText("Unknown Error", x, y, 2, gdl::Color::LightRed);
            break;
        case gdl::WiiInputStatus::BadChannel:
            defaultFont.DrawText("Bad Channel", x, y, 2, gdl::Color::LightRed);
            break;
        case gdl::WiiInputStatus::QueueEmpty:
            defaultFont.DrawText("Queue empty", x, y, 2, gdl::Color::LightRed);
            break;
        case gdl::WiiInputStatus::BadValue:
            defaultFont.DrawText("Bad Value", x, y, 2, gdl::Color::LightRed);
            break;
        case gdl::WiiInputStatus::BadConfig:
            defaultFont.DrawText("Bad Config", x, y, 2, gdl::Color::LightRed);
            break;
        default:
        break;
    }
}

void DrawJoystick(short x, short y, short size)
{
    // Draw joystick direction
    short jsize=size;
    short box = jsize/3;
    short h=box/2;
    gdl::Color::ColorValues jc = gdl::Color::Green;
    gdl::vec2 jdir = input.GetNunchukJoystickDirection(0.0f);
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
        if (input.ButtonHeld(dpad_buttons[i]))
        {
            c = gdl::Color::Red;
        }
        gdl::vec2 d=directions[i];
        gdl::DrawBoxF(x+d.x*box-h, y+ d.y*box-h, x+box+d.x*box-h, y+box+d.y*box-h, c);
    }
}

void DrawButtons(short x, short y, short size)
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
        if (input.ButtonHeld(buttons[i]))
        {
            c = active;
        }
        gdl::DrawBoxF(x + i * size, y, x+size+i*size,y+size,c);
        defaultFont.DrawText(names[i].c_str(),x+size/5+i*size,y+size/5,2,gdl::Color::White);
    }
}

void DrawControllerDebug(short x, short y)
{
    // Draw cursor
    gdl::vec2 cp = input.GetCursorPosition();
    gdl::DrawLine(cp.x-10,cp.y,cp.x+10,cp.y,gdl::Color::White);
    gdl::DrawLine(cp.x,cp.y-10,cp.x,cp.y+10,gdl::Color::White);

    DrawButtons(x, y, 20);
    y += 20;
    DrawDPad(x, y, 60);
    y += 80;
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
    gdl::Color::ColorValues active = gdl::Color::Blue;
    gdl::Color::ColorValues inactive = gdl::Color::LightBlue;
    // menu.StartMenu(x, y, w, 140, 2);
    /*
    if (menu.Button("Play Ogg", active) && musicPlaying == false)
    {
        // Play Ogg
        // TODO: Copying to tempBuffer is a bad idea
        // here because the audio is streaming.
        // How to make the data not const?
        defaultFont.Printf(x, y, 2, gdl::Color::White, "Play Music Press");
        // sampleMusic.PlayMusic(true);
        // musicPlaying = true;
    }
    if (menu.Button("Play Sound", active))
    {
        // blib.PlaySound();
        defaultFont.Printf(x, y, 2, gdl::Color::White, "Play Sound Press");
    }
    if (menu.Button("Assert", active))
    {
        gdl_assert(false, "Assert fired!");
    }
    menu.Text("Hi! I am menu.");
    menu.Text("Yellow panel");
    menu.Panel(40, gdl::Color::Yellow, 0);
    */
}

void DrawTiming(short x, short y)
{
    defaultFont.Printf(x, y, 2, gdl::Color::White, "Deltatime: %f", gdl::Delta);
    // TODO: Add gdl::Elapsed;
    defaultFont.Printf(x, y + 30, 2, gdl::Color::White, "Elapsed time: %f", sampleMusic.GetElapsed());
}

int main()
{
    init();
    // menu = gdl::MenuCreator(topaz, 8);
    float boxX = 0;
    float elapsed = 0.0f;
    short frameCount = 0;
    while(1)
    {
        WPAD_ScanPads();
        // input.StartFrame();

        if (WPAD_ButtonsDown(WPAD_CHAN_0) & WPAD_BUTTON_HOME){
            gdl::wii::Exit();
        }

        /*
		if (input.ButtonPress(WPAD_BUTTON_HOME))
		    gdl::wii::Exit();


        */
        if (WPAD_ButtonsDown(WPAD_CHAN_0) & WPAD_BUTTON_A){
            if (musicPlaying == false)
            {
                sampleMusic.PlayMusic(true);
                musicPlaying = true;
            }
        }

        gdl::PrepDisplay();
        // Input
        short top = 40;
        short left = 40;
        // DrawControllerDebug(left, top);
        left += 160;
         //DrawMenu(left, top, 120);
        left += 140;
        DrawTiming(left, top);


        // GUI
        left = 40;
        top = gdl::ScreenCenterY;

        // Draw Image
        topaz.DrawText("Gambatte minnasan!", 10, gdl::ScreenCenterY - 22, 2.0f, gdl::Color::White);
        konata.Put(left, top, gdl::Color::White, 0, 0, 0.4f);

        // Draw Sprites
        left += 200;
        DrawSprites(left, top, 4.0f);

/*

*/
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