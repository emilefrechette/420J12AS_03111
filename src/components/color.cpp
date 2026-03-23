/* Copyright (c) 2026. LetTheMiceFree. */
#include "color.h"

#include "click.h"
#include "entity.h"
#include "hover.h"

ColorComponent::ColorComponent (SDL_Color _color) : value (_color) {}

SDL_Color
ColorComponent::GetPresentValue () const
{
  return present;
}

void
ColorComponent::UpdateComponent (App *app, Entity *owner)
{
  const auto *hover = owner->GetComponent<HoverComponent> ();
  const auto *click = owner->GetComponent<ClickComponent> ();
  present = value;
  if (hover && hover->GetState () == true)
    {
      present = hover->GetActiveColor ();
    }
  if (click && click->GetState () == true)
    {
      present = click->GetActiveColor ();
    }
}
