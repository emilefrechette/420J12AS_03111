/* Copyright (c) 2026. LetTheMiceFree. */
#include "component.h"

#include "game.h"

TransformComponent::TransformComponent (const SDL_FPoint &_position, const SDL_FPoint &_size)
	: position (_position), size (_size)
{
}

void
TransformComponent::UpdateComponent (GameApp *app, Entity *entity)
{
}

ColorComponent::ColorComponent (const SDL_Color &_color) : color (_color)
{
	_default_color = color;
}

void
ColorComponent::UpdateComponent (GameApp *app, Entity *entity)
{
	if (const MouseInteractComponent *mouse_interact
		= entity->GetComponentIfExists<MouseInteractComponent> ())
		{
			switch (mouse_interact->mouse_state)
				{
				case UNSELECTED:
					color = _default_color;
					break;
				case SELECTED:
					color = (SDL_Color){ 255, 255, 255, 255 };
					break;
				case CLICKED:
					color = (SDL_Color){ 255, 0, 0, 255 };
					break;
				default:
					break;
				}
		}
}

void
MouseInteractComponent::UpdateComponent (GameApp *app, Entity *entity)
{
	if (const auto *transform = entity->GetComponentIfExists<TransformComponent> ())
		{
			if (mouse_state == CLICKED)
				{
					return;
				}
			mouse_state = UNSELECTED;
			const bool b_within_bounds_w
				= app->mouse_pos.x > transform->position.x
				  && app->mouse_pos.x < transform->position.x + transform->size.x;
			const bool b_within_bounds_h
				= app->mouse_pos.y > transform->position.y
				  && app->mouse_pos.y < transform->position.y + transform->size.y;
			if (b_within_bounds_w == true && b_within_bounds_h == true)
				{
					mouse_state = SELECTED;
				}
		}
}
PointComponent::PointComponent (const PointCategoryType _point) : score (_point) {}

void
PointComponent::UpdateComponent (GameApp *app, Entity *entity)
{
}

void
PointAccumulatorComponent::UpdateComponent (GameApp *app, Entity *entity)
{
}
