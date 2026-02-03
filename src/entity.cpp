/* Copyright (c) 2026. LetTheMiceFree. */
#include "entity.h"

void
Entity::AddComponent (const Uint64 comps)
{
	flag |= comps;
}

bool
Entity::HasComponent (const Uint64 comps) const
{
	return (flag & comps) == comps;
}

void
Entity::RemoveComponent (const Uint64 comps)
{
	flag &= ~comps;
}

void
Entity::MovementUpdate (float DeltaTime)
{
	transform.position.x += movement.velocity.x * DeltaTime;
	transform.position.y += movement.velocity.y * DeltaTime;
}

void
Entity::Render (SDL_Renderer *renderer)
{
	SDL_FRect dst
		= { transform.position.x, transform.position.y, transform.size.x, transform.size.y };
	SDL_SetRenderDrawColor (renderer, render.color.r, render.color.g, render.color.b,
							render.color.a);
	SDL_RenderFillRect (renderer, &dst);
}
