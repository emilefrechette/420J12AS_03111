/* Copyright (c) 2026. LetTheMiceFree. */
#include "transform.h"

TransformComponent::TransformComponent (SDL_FPoint _position)
    : position (_position)
{
  size.x = 0.f;
  size.y = 0.f;
}

TransformComponent::TransformComponent (SDL_FPoint _position, SDL_FPoint _size)
    : position (_position), size (_size)
{
}

void
TransformComponent::UpdateComponent (App *app, Entity *owner)
{
}

SDL_FPoint
TransformComponent::GetPosition () const
{
  return position;
}

SDL_FPoint
TransformComponent::GetSize () const
{
  return size;
}
