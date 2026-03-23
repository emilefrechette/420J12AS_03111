/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef COLOR_MODULE_H
#define COLOR_MODULE_H

#include <SDL3/SDL.h>

#include "component.h"

class ColorComponent : public Component
{
  SDL_Color value {};

public:
  ColorComponent () = default;
  ColorComponent (SDL_Color _color);
  ~ColorComponent () override = default;

  [[nodiscard]] SDL_Color GetValue() const;
  void UpdateComponent (App *app, Entity *owner) override;
};

#endif /* COLOR_MODULE_H */
