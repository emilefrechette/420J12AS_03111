/* Copyright (c) 2026. LetTheMiceFree. */
#include "characters.h"

Character_Rat::Character_Rat ()
{
	Color.r = 156;
	Color.g = 55;
	Color.b = 0;
	DefaultStepCooldown = 0.f;
	Position.x = static_cast<float> (SDL_rand (500));
	Position.y = static_cast<float> (SDL_rand (500));
	Size = (SDL_FPoint) { 70.f, 42.f };
}

void
Character_Rat::ExecuteUpdate (float DeltaTime)
{
	Position.x += 100.f * DeltaTime;
}

Character_Punk::Character_Punk ()
{
	Color.r = 26;
	Color.g = 124;
	Color.b = 24;
	DefaultStepCooldown = 0.f;
	Position.x = static_cast<float> (SDL_rand (500));
	Position.y = static_cast<float> (SDL_rand (500));
	Size = (SDL_FPoint) { 22.f, 90.f };
}

void
Character_Punk::ExecuteUpdate (float DeltaTime)
{
	Position.y += 10.f * DeltaTime;
}
