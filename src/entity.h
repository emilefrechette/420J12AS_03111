/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef ENTITY_MODULE_H
#define ENTITY_MODULE_H
#include "component.h"
#include "observer.h"

#include <SDL3/SDL_render.h>

class Entity : public IObserver
{
  public:
	TransformComponent transform;
	ColorComponent appearance;

	Entity (float x, float y);
	void Update (float dt);
	void Draw (SDL_Renderer *renderer);

	void OnNotify () override;
};

#endif /* ENTITY_MODULE_H */
