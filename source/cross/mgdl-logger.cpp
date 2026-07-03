#include <stdio.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-console.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-memory.h>

static LogLevel level_ = All;
#define LINE_COUNT 256
static char* messages[LINE_COUNT];
static bool saveLinesOn = false;
static int nextSaveIndex = 0;
static int lineAmount = 0;
static const int LINE_LENGTH  = 256;

static char empty[1] = "";

// Own buffer for logger
static char logBuffer[MGDL_PRINTF_BUFFER_SIZE];

char* mgdl_GetLogBuffer()
{
	return logBuffer;
}

/*
 * @brief Writes the formatted text to a buffer.
 * @param format Format string.
 */
#define MGDL_PRINTF_TO_LOGBUFFER(format) \
	memset(mgdl_GetLogBuffer(), '\0', MGDL_PRINTF_BUFFER_SIZE);\
	va_list args;\
	va_start(args, format); \
	vsnprintf(mgdl_GetLogBuffer(), MGDL_PRINTF_BUFFER_SIZE, format, args); \
	va_end(args);\

static void Log_Print(LogLevel lvl, const char* text)
{
	if (saveLinesOn)
	{
		strncpy(messages[nextSaveIndex], text, LINE_LENGTH);
		nextSaveIndex = (nextSaveIndex + 1) % lineAmount;
	}
	switch(lvl)
	{
		case Error:
		Console_Print(ConsoleText_Magenta, "[Error] ");
		break;
		case Warning:
		Console_Print(ConsoleText_Cyan, "[Warning] ");
		break;
		case Info:
			// Nop
		break;
		case All:
			// Nop
		break;
		case None:
			// Nop
		break;
	};
	printf("%s", text);
}

void Log_SetLevel(LogLevel lvl)
{
	level_ = lvl;
}

void Log_SetLineIndexTo(int index)
{
	if (index >=0 && index < lineAmount)
	{
		nextSaveIndex = index;
	}
}
int Log_GetLineIndex()
{
	return nextSaveIndex;
}

void Log_SaveLines(int amount)
{
	Log_InfoF("Log will save %d lines\n", amount);
	lineAmount = amount;
	for (int i = 0; i < lineAmount; i++)
	{
		messages[i] = (char*)mgdl_AllocateGeneralMemory(sizeof(char) * LINE_LENGTH);
		messages[i][0] = '\0';
	}
	saveLinesOn = true;
}
char* Log_GetLine(int index)
{
	if (saveLinesOn)
	{
		return messages[index%lineAmount];
	}
	else return empty;
}
char* Log_GetLastLine(int index)
{
	if (saveLinesOn)
	{
		int readI = nextSaveIndex-index;
		if (readI < 0)
		{
			readI = 0;
		}
		return messages[readI];
	}
	else return empty;
}

void Log_Info(const char* text)
{
	if (level_ >= Info)
	{
		Log_Print(Info, text);
	}
}

void Log_InfoF(const char* fmt, ...)
{
	if (level_ >= Info)
	{
		MGDL_PRINTF_TO_LOGBUFFER(fmt);
		Log_Print(Info, mgdl_GetLogBuffer());
	}
}

void Log_Warning(const char* text)
{
	if (level_ >= Warning)
	{
		Log_Print(Warning, text);
	}
}

void Log_WarningF(const char* fmt, ...)
{
	if (level_ >= Warning)
	{
		MGDL_PRINTF_TO_LOGBUFFER(fmt);
		Log_Print(Warning, mgdl_GetLogBuffer());
	}
}

void Log_Error(const char* text)
{
	if (level_ >= Error)
	{
		Log_Print(Error, text);
	}
}

void Log_ErrorF(const char* fmt, ...)
{
	if (level_ >= Error)
	{
		MGDL_PRINTF_TO_LOGBUFFER(fmt);
		Log_Print(Error, mgdl_GetLogBuffer());
	}
}

