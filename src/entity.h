/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef ENTITY_MODULE_H
#define ENTITY_MODULE_H

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

class Entity
{
	float StepCooldown = 0.f;
	float DefaultStepCooldown = 90.f;

  public:
	Entity ();
	virtual ~Entity ();

	void Color (SDL_Renderer *Renderer) const;
	[[nodiscard]] SDL_FRect Destination () const;

	void Update (float DeltaTime);
	virtual void ExecuteUpdate (float DeltaTime) = 0;

  protected:
	SDL_FPoint position;
	SDL_FPoint size;
	SDL_Color color;
};

#endif /* ENTITY_MODULE_H */
