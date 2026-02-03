/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef COMPONENTS_MODULE_H
#define COMPONENTS_MODULE_H

#include "state.h"

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>

/**
 * @headerfile components.h
 * @brief Ce module contient différentes classes représentant des types de composant. Ces classes
 * sont utilisées dans un style orienté objet pour représenter le patron de conception du composant
 * (component pattern).
 */

class TransformComponent;

typedef enum
{
	HEALTH = 1ULL << 0,
	MAGIC = 1ULL << 1,
	MOVEMENT = 1ULL << 2,
	RENDER = 1ULL << 3,
	TRANSFORM = 1ULL << 4,
} ComponentType;

class Component
{
  public:
	virtual ~Component () = default;
};

class HealthComponent : public Component
{
  public:
	Sint32 current_health = 0;
	Sint32 max_health = 0;

	HealthComponent () = default;
};

class MovementComponent : public Component
{

public:
	SDL_FPoint velocity = { 0.f, 0.f };

	MovementComponent () = default;
};

class MagicComponent : public Component
{

public:
	Sint32 damage = 0;
	MagicType *type = new InvalidMagicType();

	MagicComponent () = default;
};

class RenderComponent : public Component
{
  public:
	SDL_Color color = { 0, 0, 0, 0 };

	RenderComponent () = default;
};

class TransformComponent : public Component
{

public:
	SDL_FPoint position = { 0.f, 0.f };
	SDL_FPoint size = { 0.f, 0.f };

	TransformComponent () = default;
};

#endif /* COMPONENTS_MODULE_H */
