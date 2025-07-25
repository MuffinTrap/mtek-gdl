// muffintrap: this function is used by the library
// and you can use it too!
#include <stdio.h>
#include <stdarg.h>
#include <wiiuse/wpad.h>
#include <mgdl/wii/mgdl-wii.h>
#include <mgdl/mgdl-console.h>

void AssertFunctionPrintf(const char* filename, int lineNumber, const char* message, ...)
{
    char	buff[256];
    va_list args;

    va_start(args, message);
    vsprintf(buff, message, args);
    va_end(args);

    gdl::ConsoleMode();
    Console_Printf(ConsoleText_Cyan, "Assert failed! %s:%d:%s\n", filename, lineNumber, buff);
    printf("Press HOME to quit or A to continue\n");
    while (true)
    {
        WPAD_ScanPads();
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME)
        {
            // Return to homebrew menu
            break;
        }
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_A)
        {
            return;
        }

        VIDEO_WaitVSync();
    }

    VIDEO_SetBlack(TRUE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    VIDEO_WaitVSync();
    exit(0);
}

void AssertFunctionPrint(const char* filename, int lineNumber, const char* message)
{
    AssertFunctionPrintf(filename, lineNumber, message);
}
