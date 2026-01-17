#include <stdio.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-util.h>

static LogLevel level_ = All;
#define LINE_COUNT 256
static char* messages[LINE_COUNT];
static bool saveLinesOn = false;
static int nextSaveIndex = 0;
static int lineAmount = 0;
static const int LINE_LENGTH  = 256;



void Log_SetLevel(LogLevel lvl)
{
	level_ = lvl;
}

void Log_SaveLines(int amount)
{
	Log_InfoF("Log will save %d lines\n", amount);
	lineAmount = amount;
	for (int i = 0; i < lineAmount; i++)
	{
		messages[i] = (char*)malloc(sizeof(char) * LINE_LENGTH);
		messages[i][0] = '\0';
	}
	saveLinesOn = true;
}
char* Log_GetLine(int index)
{
	return messages[index%lineAmount];
}
char* Log_GetLastLine(int index)
{
	int readI = nextSaveIndex-index;
	if (readI < 0)
	{
		readI = 0;
	}
	return messages[readI];
}

void Log_Info(const char* text)
{
	if (level_ >= Info)
	{
		_Log_Print(text);
	}
}

void Log_InfoF(const char* fmt, ...)
{
	if (level_ >= Info)
	{
		MGDL_PRINTF_TO_BUFFER(fmt);
		_Log_Print(mgdl_GetPrintfBuffer());
	}

}

void Log_Warning(const char* text)
{
	if (level_ >= Warning)
	{
		_Log_Print(text);
	}
}

void Log_WarningF(const char* fmt, ...)
{
	if (level_ >= Warning)
	{
		MGDL_PRINTF_TO_BUFFER(fmt);
		_Log_Print(mgdl_GetPrintfBuffer());
	}

}

void Log_Error(const char* text)
{
	if (level_ >= Error)
	{
		_Log_Print(text);
	}

}

void Log_ErrorF(const char* fmt, ...)
{
	if (level_ >= Error)
	{
		MGDL_PRINTF_TO_BUFFER(fmt);
		_Log_Print(mgdl_GetPrintfBuffer());
	}

}

void _Log_Print(const char* text)
{
	if (saveLinesOn)
	{
		strncpy(messages[nextSaveIndex], text, LINE_LENGTH);
		nextSaveIndex = (nextSaveIndex + 1) % lineAmount;
	}
	printf("%s", text);
}
