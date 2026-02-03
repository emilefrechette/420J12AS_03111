/* Copyright (c) 2026. LetTheMiceFree. */
#include "state.h"

void
MagicType::Affect (Entity *entity)
{
}

void
InvalidMagicType::Affect (Entity *entity)
{
	MagicType::Affect (entity);
}
SDL_Color
InvalidMagicType::GetColor ()
{
	return (SDL_Color){55, 33, 22, 255};
}
void
FireMagicType::Affect (Entity *entity)
{
}
SDL_Color
FireMagicType::GetColor ()
{
	return (SDL_Color){255, 0, 0, 255};
}

void
IceMagicType::Affect (Entity *entity)
{
}
SDL_Color
IceMagicType::GetColor ()
{
	return (SDL_Color){0, 125, 255, 255};
}

void
PoisonMagicType::Affect (Entity *entity)
{
}
SDL_Color
PoisonMagicType::GetColor ()
{
	return (SDL_Color){55, 255, 55, 255};
}
