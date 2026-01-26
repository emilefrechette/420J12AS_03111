/* Copyright (c) 2026. LetTheMiceFree. */
#include "characters.h"

Character_Rat::Character_Rat ()
{
	color.r = 156;
	color.g = 55;
	color.b = 0;
	position.x = static_cast<float> (SDL_rand (500));
	position.y = static_cast<float> (SDL_rand (500));
	size = (SDL_FPoint) { 70.f, 42.f };
}

void
Character_Rat::ExecuteUpdate (float DeltaTime)
{
	position.x += 100.f * DeltaTime;
}

Character_Punk::Character_Punk ()
{
	color.r = 26;
	color.g = 124;
	color.b = 24;
	position.x = static_cast<float> (SDL_rand (500));
	position.y = static_cast<float> (SDL_rand (500));
	size = (SDL_FPoint) { 22.f, 90.f };
}

void
Character_Punk::ExecuteUpdate (float DeltaTime)
{
	position.y += 10.f * DeltaTime;
}
