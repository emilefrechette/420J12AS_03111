/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef TRANSFORM_MODULE_H
#define TRANSFORM_MODULE_H
#include "component.h"

#include <SDL3/SDL.h>

class TransformComponent : public Component
{
  SDL_FPoint position{ 0 };
  SDL_FPoint size{ 0 };

public:
  TransformComponent () = default;
  TransformComponent (SDL_FPoint _position);
  TransformComponent (SDL_FPoint _position, SDL_FPoint _size);
  ~TransformComponent () override = default;

  void UpdateComponent(App *app, Entity *owner) override;

  [[nodiscard]] SDL_FPoint GetPosition () const;
  [[nodiscard]] SDL_FPoint GetSize () const;
};

#endif /* TRANSFORM_MODULE_H */
