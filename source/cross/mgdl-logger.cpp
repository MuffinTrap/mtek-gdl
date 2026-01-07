#include <stdio.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-util.h>

static LogLevel level_ = All;

void Log_SetLevel(LogLevel lvl)
{
level_ = lvl;
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
	printf("%s", text);
}
