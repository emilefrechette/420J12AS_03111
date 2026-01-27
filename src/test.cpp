/* Copyright (c) 2026. LetTheMiceFree. */
#include "test.h"

#include <SDL3/SDL_log.h>

void
PrintTestMessage ()
{
	SDL_Log("Message test %d %.1f %s", 1, 10.f, "allo");
	SDL_LogWarn(0, "Warning message!");
	SDL_LogError(1, "Error message!");
	SDL_LogCritical(1, "Critical error!");
}

void
TestObject::PrintTestMessage ()
{
	SDL_Log("Message test");
	SDL_LogWarn(0, "Warning message!");
	SDL_LogError(1, "Error message!");
	SDL_LogCritical(1, "Critical error!");
}
