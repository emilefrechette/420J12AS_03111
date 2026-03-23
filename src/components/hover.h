/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef HOVER_MODULE_H
#define HOVER_MODULE_H

#include "component.h"

#include <SDL3/SDL.h>

class App;
class Entity;

class HoverComponent : public Component
{
  bool b_state = false;
  SDL_Color active_color{ 255u, 255u, 255u, 255u };

public:
  HoverComponent () = default;
  HoverComponent (SDL_Color _active_color);
  ~HoverComponent () override = default;

  [[nodiscard]] bool GetState() const;
  [[nodiscard]] SDL_Color GetActiveColor() const;

  void UpdateComponent (App *app, Entity *owner) override;
};

#endif /* HOVER_MODULE_H */
