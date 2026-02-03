/* Copyright (c) 2026. LetTheMiceFree. */
#include "characters.h"

Character_Rat::Character_Rat ()
{
	AddComponent (HEALTH);
	AddComponent (MOVEMENT);
	movement.velocity = (SDL_FPoint){2.f, 0.f};
	AddComponent (RENDER);
	render.color = (SDL_Color){ 55, 128, 24, 255 };
	AddComponent (TRANSFORM);
	transform.position
		= (SDL_FPoint){ static_cast<float> (SDL_rand (500)), static_cast<float> (SDL_rand (500)) };
	transform.size = (SDL_FPoint){ 40.f, 64.f };
}

Character_Punk::Character_Punk ()
{
	AddComponent (HEALTH);
	AddComponent (MOVEMENT);
	AddComponent (MAGIC);
	magic.damage = 8;
	magic.type = new IceMagicType ();
	AddComponent (RENDER);
	render.color = (SDL_Color){ 122, 44, 65, 255 };
	AddComponent (TRANSFORM);
	transform.position
		= (SDL_FPoint){ static_cast<float> (SDL_rand (500)), static_cast<float> (SDL_rand (500)) };
	transform.size = (SDL_FPoint){ 80.f, 38.f };
}
