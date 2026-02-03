/* Copyright (c) 2026. LetTheMiceFree. */
#ifndef STATE_OOP_MODULE_H
#define STATE_OOP_MODULE_H

#include <SDL3/SDL_pixels.h>

class Entity;

class MagicType
{
public:
	MagicType() = default;
	virtual ~MagicType() = default;
	virtual void Affect(Entity *entity);

	virtual SDL_Color GetColor() = 0;
};

class InvalidMagicType : public MagicType
{
public:
	InvalidMagicType() = default;
	void Affect(Entity *entity) override;

	SDL_Color GetColor() override;
};

class FireMagicType : public MagicType
{
public:
	FireMagicType() = default;
	void Affect(Entity *entity) override;

	SDL_Color GetColor() override;
};

class IceMagicType : public MagicType
{
public:
	IceMagicType() = default;
	void Affect(Entity *entity) override;

	SDL_Color GetColor() override;
};

class PoisonMagicType : public MagicType
{
public:
	PoisonMagicType() = default;
	void Affect(Entity *entity) override;

	SDL_Color GetColor() override;
};

#endif // STATE_OOP_MODULE_H
