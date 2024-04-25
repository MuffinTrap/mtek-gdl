#include <mgdl-wii.h>
#include <wiiuse/wpad.h>
#include <string>

// Image
// SpriteSet
// FFont
// Ogg
// Sound
static gdl::Font defaultFont;


// For reasons beyond my understading
// the input reading cannot be in a 
// classless function...???
static gdl::WiiInput input;

void init()
{
    fatInitDefault();
	gdl::InitSystem(gdl::ModeMPAL, gdl::Aspect16x9, gdl::HiRes);
    gdl::SetClearColor(gdl::Color::Black);
    input.Init();
    gdl::ConsoleMode();

    defaultFont.LoadFontMem(gdl::DefaultFontData);
}

int main()
{
    init();
    float boxX = 0;
    while(1)
    {
        input.StartFrame();

		if (input.ButtonPress(WPAD_BUTTON_HOME))
		    gdl::wii::Exit();

        gdl::PrepDisplay();
        gdl::DrawBoxF(boxX, gdl::ScreenYres-120, boxX+60, gdl::ScreenYres-60, gdl::Color::LightGreen);
        // Input
            // Draw cursor
            gdl::vec2 cp = input.GetCursorPosition();
            gdl::DrawLine(cp.x-10,cp.y,cp.x+10,cp.y,gdl::Color::White);
            gdl::DrawLine(cp.x,cp.y-10,cp.x,cp.y+10,gdl::Color::White);

            gdl::vec2 inputPos = gdl::vec2(20,40);

            short inputY = 80;

            short bpos = 10;
            short dpos = 80;
            short jpos = 120;
            short fpos = 220;
            short deltapos = 400;
            short box=20;
            short h=box/2;
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
            std::string names[] ={ "A", "B", "+", "-", "1", "2" };
            for(int i = 0; i < 6;i++ )
            {
                gdl::Color::ColorValues c = inactive;
                if (input.ButtonHeld(buttons[i]))
                {
                    c = active;
                }
                gdl::DrawBoxF(bpos, inputY+i*20, bpos+20,inputY+i*20+20,c);
                defaultFont.DrawText(names[i].c_str(),bpos+4,inputY+i*20+4,2,gdl::Color::White);
            }
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
                gdl::Color::ColorValues c = inactive;
                if (input.ButtonHeld(dpad_buttons[i]))
                {
                    c = active;
                }
                gdl::vec2 d=directions[i];
                gdl::DrawBoxF(dpos+d.x*box-h, inputY+d.y*box-h,dpos+box+d.x*box-h,inputY+box+d.y*box-h, c);
            }
            
            // Draw joystick direction
            short jsize=60;
            gdl::Color::ColorValues jc = inactive;
            gdl::vec2 jdir = input.GetNunchukJoystickDirection(0.0f);
            short jleft=jpos+jsize/2+jdir.x*box-h;
            short jtop =inputY+jsize/2+jdir.y*box-h;
            gdl::DrawBox(jpos, inputY, jpos+jsize, inputY+jsize, jc);
            gdl::DrawBoxF(jleft,jtop,jleft+box,jtop+box,jc);


        // GUI
        gdl::MenuCreator menu(defaultFont);
        menu.StartMenu(fpos, inputY, 200, 140, 2);
        if (menu.Button("Play Ogg", active))
        {
            // Play Ogg
        }
        if (menu.Button("Play Sound", active))
        {
            // Play Ogg
        }
        if (menu.Button("Assert", active))
        {
            // Play Ogg
        }

        defaultFont.Printf(deltapos, inputY, 2, gdl::Color::White, "Deltatime: %f", gdl::Delta);

        // Draw Image

        // Draw Sprites

        gdl::Display();

        boxX += gdl::Delta;
        if (boxX > gdl::ScreenXres)
        {
            boxX = 0;
        }
    }
}