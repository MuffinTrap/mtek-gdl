#include <mgdl/mgdl-console.h>
#include <mgdl/mgdl-util.h>
#include <stdio.h>
#include <string.h>

void Console_Print(ConsoleTextColor color, const char* text)
{
    printf("\033[%dm%s\033[0m", (int)color, text);
}

void Console_Printf(ConsoleTextColor color, const char* format, ...)
{
	MGDL_PRINTF_TO_BUFFER(format);

    Console_Print(color, mgdl_GetPrintfBuffer());
}

void Console_SetTextColor(ConsoleTextColor color)
{
    printf("\033[%dm", (int)color);
}
void Console_ResetTextColor()
{
    printf(MGDL_CONSOLECOLOR_RESET);
}
