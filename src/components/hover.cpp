/* Copyright (c) 2026. LetTheMiceFree. */
#include "hover.h"

#include "app.h"
#include "entity.h"
#include "transform.h"

HoverComponent::HoverComponent (const SDL_Color _active_color)
    : active_color (_active_color)
{
}

bool
HoverComponent::GetState () const
{
  return b_state;
}

SDL_Color
HoverComponent::GetActiveColor () const
{
  return active_color;
}

void
HoverComponent::UpdateComponent (App *app, Entity *owner)
{
  const TransformComponent *transform
      = owner->GetComponent<TransformComponent> ();

  if (!transform)
    {
      return;
    }

  const SDL_FRect dst = {
    transform->GetPosition ().x,
    transform->GetPosition ().y,
    transform->GetSize ().x,
    transform->GetSize ().y,
  };
  SDL_FPoint mouse_pos = app->GetMousePosition ();
  if (mouse_pos.x > dst.x && mouse_pos.x < dst.x + dst.w && mouse_pos.y > dst.y
      && mouse_pos.y < dst.y + dst.h)
    {
      b_state = true;
    }
  else
    {
      b_state = false;
    }
}
