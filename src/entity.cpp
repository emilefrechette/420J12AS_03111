/* Copyright (c) 2026. LetTheMiceFree. */
#include "entity.h"

Entity::Entity () : Position ({ 0.f, 0.f }), Size ({ 0.f, 0.f }), Color ({ 125, 125, 125, 255 }) {}

Entity::~Entity () = default;

void
Entity::SetRenderColorToEntity (SDL_Renderer *Renderer) const
{
	SDL_SetRenderDrawColor (Renderer, Color.r, Color.g, Color.b, Color.a);
}

SDL_FRect
Entity::Destination () const
{
	return (SDL_FRect){ Position.x, Position.y, Size.x, Size.y };
}

void
Entity::Update (const float DeltaTime)
{
	/* Execute Update uniquement lorsque le délai est expiré. */
	if (StepCooldown > 0.f)
		{
			StepCooldown -= DeltaTime * 1000.f;
			return;
		}
	ExecuteUpdate (DeltaTime);

	/* Rafraîchi le délai. */
	StepCooldown = DefaultStepCooldown;
}
