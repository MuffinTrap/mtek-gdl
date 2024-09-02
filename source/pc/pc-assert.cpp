#include <mgdl/pc/mgdl-pc-assert.h>

#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <cassert>
void AssertFunctionPrintf(const char* filename, int lineNumber, bool &ignoreToggle, const char* message, ...)
{
	char	buff[256];
	va_list args;

	va_start(args, message);
	vsnprintf(buff, 256, message, args);
	va_end(args);

	printf("Assert failed! %s:%d:%s\n", filename, lineNumber, buff);
	printf("Input i to ignore this assert. Input nothing to proceed to assert handler\n");
	char answer = '\0';
	std::cin >> answer;
	if (answer == 'i')
	{
		ignoreToggle = true;
		return;
	}
}
void AssertFunctionPrint(const char* filename, int lineNumber, bool &ignoreToggle, const char* message)
{
	printf("Assert failed! %s:%d:%s\n", filename, lineNumber, message);
	printf("Input i to ignore this assert. Input nothing to proceed to assert handler\n");
	char answer = '\0';
	std::cin >> answer;
	if (answer == 'i')
	{
		ignoreToggle = true;
		return;
	}
	assert(false);
}

