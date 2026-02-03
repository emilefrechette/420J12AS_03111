/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef ENTITY_MODULE_H
#define ENTITY_MODULE_H

#include "components.h"

#include <SDL3/SDL_render.h>

/**
 * @brief Classe de base pour représenter une entité dans le jeu.
 */
class Entity
{
	Uint64 flag = 0u;

  public:
	Entity () = default;
	virtual ~Entity () = default;

	HealthComponent health;
	MagicComponent magic;
	MovementComponent movement;
	RenderComponent render;
	TransformComponent transform;

	void AddComponent(Uint64 comps);
	[[nodiscard]] bool HasComponent(Uint64 comps) const;
	void RemoveComponent(Uint64 comps);

	virtual void MovementUpdate(float DeltaTime);
	void Render(SDL_Renderer *renderer);
};

#endif /* ENTITY_MODULE_H */
