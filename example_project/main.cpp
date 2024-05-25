#include <mgdl-wii.h>
#include <wiiuse/wpad.h>

#include "mgdl-input-wii.h"
#include "template.h"

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
    {
        Template temp = Template();
        temp.Init();

        gdl::ConsoleMode();
        /*
        Uncomment to see console messages
        before game starts
        while(1)
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
                gdl::wii::Exit();
            }

            if (gameRunning)
            {
                temp.Update();
            }

            gdl::PrepDisplay();
            if (gameRunning)
            {
                temp.Draw();
            }
            gdl::Display();
        }
    }
}
