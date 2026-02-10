/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef COMPONENT_MODULE_H
#define COMPONENT_MODULE_H

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>

struct TransformComponent
{
	SDL_FRect rect;
	float velocityY;
};

struct ColorComponent
{
	SDL_Color color;
};

#endif /* COMPONENT_MODULE_H */
