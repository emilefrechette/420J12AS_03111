/* Copyright (c) 2026. LetTheMiceFree. */
#ifndef PLAYER_MODULE_H
#define PLAYER_MODULE_H

#include "entity.h"

#include <vector>

class Player : public Entity
{
public:
	Player ();

	void Shoot(std::vector<Entity *> &ents, SDL_Point dir);
};

class Bullet : public Entity
{

public:
	Bullet (SDL_FPoint spawn, SDL_Point dir, SDL_Color color);
};

#endif // PLAYER_MODULE_H
