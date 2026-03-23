/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef COLOR_MODULE_H
#define COLOR_MODULE_H

#include <SDL3/SDL.h>

#include "component.h"

class ColorComponent : public Component
{
  SDL_Color value {};
  SDL_Color present {};

public:
  ColorComponent () = default;
  ColorComponent (SDL_Color _color);
  ~ColorComponent () override = default;

  [[nodiscard]] SDL_Color GetPresentValue() const;
  void UpdateComponent (App *app, Entity *owner) override;
};

#endif /* COLOR_MODULE_H */
