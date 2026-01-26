/* Copyright (c) 2026. LetTheMiceFree. */
#include "entity.h"

Entity::Entity () : position ({ 0.f, 0.f }), size ({ 0.f, 0.f }), color ({ 125, 125, 125, 255 }) {}

Entity::~Entity () = default;

void
Entity::Color (SDL_Renderer *Renderer) const
{
	SDL_SetRenderDrawColor (Renderer, color.r, color.g, color.b, color.a);
}

SDL_FRect
Entity::Destination () const
{
	return (SDL_FRect){ position.x, position.y, size.x, size.y };
}

void
Entity::Update (const float DeltaTime)
{
	// if (StepCooldown > 0.f)
	// 	{
	// 		StepCooldown -= DeltaTime * 1000.f;
	// 		return;
	// 	}
	ExecuteUpdate (DeltaTime);
	// StepCooldown = DefaultStepCooldown;
}
