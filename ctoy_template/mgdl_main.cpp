#include <mgdl.h>
#include <mgdl/mgdl-rocket.h>
#include "main.h"

// Called before render()
void update()
{
    Rocket_UpdateRow();
    if (WiiController_ButtonPress(Platform_GetController(0), WiiButtons::ButtonHome))
    {
        ctoy_end();
        mgdl_DoProgramExit();
    }
}

int main()
{
    Music* defaultMusic = mgdl_LoadWav("song.wav");
    // Init rocket
    Rocket_Init(defaultMusic, 1, 1);

    u32 flags = 0;
    mgdl_InitSystem(MGDL_PLATFORM,
        gdl::ScreenAspect::Screen4x3,
        ctoy_begin,
        update,
        ctoy_update,
        flags
    );


}
