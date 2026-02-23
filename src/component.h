/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef COMPONENT_MODULE_H
#define COMPONENT_MODULE_H

#include <SDL3/SDL.h>

class GameApp;
class Entity;

class Component
{
  public:
	Component () = default;
	virtual ~Component () = default;

	virtual void UpdateComponent (GameApp *app, Entity *entity) = 0;
};

class TransformComponent : public Component
{
  public:
	SDL_FPoint position = (SDL_FPoint){ 0 };
	SDL_FPoint size = (SDL_FPoint){ 0 };

	TransformComponent () = default;
	TransformComponent (const SDL_FPoint &_position, const SDL_FPoint &_size);
	~TransformComponent () override = default;

	void UpdateComponent (GameApp *app, Entity *entity) override;
};

class ColorComponent : public Component
{
	SDL_Color _default_color = (SDL_Color){ 0 };
  public:
	SDL_Color color = (SDL_Color){ 0 };

	ColorComponent () = default;
	ColorComponent(const SDL_Color &_color);
	~ColorComponent () override = default;

	void UpdateComponent (GameApp *app, Entity *entity) override;
};

enum MouseInteractState
{
	UNSELECTED,
	SELECTED,
	CLICKED
};

class MouseInteractComponent : public Component
{
public:
	MouseInteractState mouse_state = UNSELECTED;

	MouseInteractComponent () = default;
	~MouseInteractComponent () override = default;

	void UpdateComponent (GameApp *app, Entity *entity) override;
};


enum PointCategoryType
{
	NORMAL = 1,
	SUPER = 3,
	OMEGA = 10
};

class PointComponent : public Component
{
public:
	PointCategoryType score = NORMAL;

	PointComponent() = default;
	PointComponent(PointCategoryType _point);
	~PointComponent() override = default;

	void UpdateComponent (GameApp *app, Entity *entity) override;
};

class PointAccumulatorComponent : public Component
{
public:
	Sint32 total = 0;

	PointAccumulatorComponent() = default;
	~PointAccumulatorComponent() override = default;

	void UpdateComponent (GameApp *app, Entity *entity) override;
};

#endif /* COMPONENT_MODULE_H */
