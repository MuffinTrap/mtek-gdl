#include <mgdl-wii.h>
#include <wiiuse/wpad.h>

// Image
// SpriteSet
// FFont
// Ogg
// Sound


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
}

int main()
{
    init();
    float linex = 0;
    while(1)
    {
        input.StartFrame();

        // This Crashes the emulator
		if (input.ButtonPress(WPAD_BUTTON_HOME))
		    gdl::wii::Exit();

        // This Works
		// if ((WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME))
			//gdl::wii::Exit();

        gdl::PrepDisplay();

        gdl::DrawLine(linex, 0, gdl::ScreenXres, gdl::ScreenYres, gdl::Color::LightGreen);

        // Input
            // Draw cursor
            // Draw button states
            // Draw joystick direction

        // GUI

        // Play Ogg

        // Play Sound

        // Draw Image

        // Draw Sprites

        // Fire assert

        gdl::Display();

        linex += 0.1f * gdl::Delta;
    }
}