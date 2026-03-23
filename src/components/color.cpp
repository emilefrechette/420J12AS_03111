/* Copyright (c) 2026. LetTheMiceFree. */
#include "color.h"

ColorComponent::ColorComponent (SDL_Color _color) : value (_color)
{

}

SDL_Color
ColorComponent::GetValue () const
{
  return value;
}

void
ColorComponent::UpdateComponent (App *app, Entity *owner)
{
}
