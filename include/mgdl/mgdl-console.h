#pragma once

/**
* @file mgdl-console.h
* @brief Functions to use the Wii/Linux/Win/etc console
*/

enum ConsoleTextColor
{
    ConsoleText_Black = 30,
    ConsoleText_Red,
    ConsoleText_Green,
    ConsoleText_Brown,
    ConsoleText_Blue,
    ConsoleText_Magenta,
    ConsoleText_Cyan,
    ConsoleText_White,
    ConsoleText_Default = 39
};
typedef enum ConsoleTextColor ConsoleTextColor;

// https://www.man7.org/linux/man-pages/man4/console_codes.4.html

#ifdef __cplusplus
extern "C" {
#endif

void Console_Print(ConsoleTextColor color, const char* text);

void Console_Printf(ConsoleTextColor color, const char* format, ...);

#ifdef __cplusplus
}
#endif
