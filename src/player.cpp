/* Copyright (c) 2026. LetTheMiceFree. */

#include "player.h"

Player::Player ()
{
	AddComponent (HEALTH);
	AddComponent (MOVEMENT);
	movement.velocity = (SDL_FPoint){ 0.f, 0.f };
	AddComponent (MAGIC);
	magic.type = new PoisonMagicType ();
	AddComponent (RENDER);
	render.color = (SDL_Color){ 255, 125, 0, 255 };
	AddComponent (TRANSFORM);
	transform.position = (SDL_FPoint){ 200, 200 };
	transform.size = (SDL_FPoint){ 64.f, 64.f };
}

void
Player::Shoot (std::vector<Entity *> &ents, SDL_Point dir)
{
	ents.push_back (new Bullet (transform.position, dir, magic.type->GetColor ()));
}

Bullet::Bullet (SDL_FPoint spawn, SDL_Point dir, SDL_Color color)
{
	AddComponent (MOVEMENT);
	movement.velocity = (SDL_FPoint){ 120.f * dir.x, 120.f * dir.y };
	AddComponent (RENDER);
	render.color = color;
	AddComponent (TRANSFORM);
	transform.position = spawn;
	transform.size = (SDL_FPoint){ 16.f, 16.f };
}
