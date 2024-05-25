#include <mgdl-wii.h>
#include <wiiuse/wpad.h>

#include "mgdl-input-wii.h"

void init()
{
    fatInitDefault();
	gdl::InitSystem(gdl::ModeAuto, gdl::AspectAuto, gdl::HiRes);
    gdl::SetClearColor(gdl::Color::Black);
    gdl::WiiInput::Init();
    gdl::ConsoleMode();
}

int main()
{
    init();

    gdl::ConsoleMode();

    // Init your Game here
    /*
             Uncomment to see console messages
             before game starts
    */
    /*
        while(true)
        {
            gdl::WiiInput::StartFrame();
            if (gdl::WiiInput::ButtonPress(WPAD_BUTTON_HOME)){
                break;
            }
            VIDEO_WaitVSync();
        }
    */
    bool gameRunning = true;
    while(true)
    {
        gdl::WiiInput::StartFrame();

        if (gdl::WiiInput::ButtonPress(WPAD_BUTTON_HOME)){
            gameRunning = false;
            // This starts the exit process
            gdl::wii::Exit();
        }

        if (gameRunning)
        {
            // Update your game here
        }


        gdl::PrepDisplay();

        if (gameRunning)
        {
            // Draw your game here
        }
        gdl::Display();
    }
}
