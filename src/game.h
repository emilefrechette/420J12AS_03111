/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef GAME_MODULE_H
#define GAME_MODULE_H

#include "SDL3/SDL.h"
#include <string>

#include "entity.h"

static const std::string AppTitle = "Solution - Pratique Examen de mi-session";
static constexpr size_t MAX_SAMPLES = 100;

class GameApp final
{
	std::vector<float> frames;

  public:
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	std::vector<Entity *> entities = std::vector<Entity *> ();
	float avg_fps = 0.0f;
	SDL_FPoint mouse_pos = (SDL_FPoint){ 0 };
	Entity *bag = nullptr;

	GameApp ();
	~GameApp ();

	void CalculateFPS (float DeltaTime);
};

class Drawable
{
  public:
	Drawable () = default;
	virtual ~Drawable () = default;

	virtual void Draw (GameApp *app);
};

class TokenEntity : public Entity, public Drawable
{
  public:
	TokenEntity (const GameApp *app);
};

class BagEntity : public Entity, public Drawable
{
  public:
	BagEntity (const GameApp *app);
};

#endif /* GAME_MODULE_H */
