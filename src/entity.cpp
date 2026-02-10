/* Copyright (c) 2026. LetTheMiceFree. */
#include "entity.h"

Entity::Entity (float x, float y)
{
	const float size = (float)(SDL_rand (10) + 5);
	transform = { { x, y, size, size }, (float)(SDL_rand (50) + 20) };
	appearance = { { 255, 255, 255, 255 } };
}
void
Entity::Update (float dt)
{
	transform.rect.y += transform.velocityY * dt;
	if (transform.rect.y > 600)
		transform.rect.y = -10;
}

void
Entity::Draw (SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor (renderer, appearance.color.r, appearance.color.g, appearance.color.b,
							255);
	SDL_RenderFillRect (renderer, &transform.rect);
}

void
Entity::OnNotify ()
{
	appearance.color.r = SDL_rand (256);
	appearance.color.g = SDL_rand (256);
	appearance.color.b = SDL_rand (256);
}
