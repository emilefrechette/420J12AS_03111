/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef CLICK_MODULE_H
#define CLICK_MODULE_H

#include "component.h"

#include <SDL3/SDL.h>

class Entity;
class App;

enum ClickEffect
{
  CLICK_EFFECT_NONE,
  CLICK_EFFECT_INC_SCORE,
  CLICK_EFFECT_SEND_SCORE,
  CLICK_EFFECT_GET_HIGHSCORE,
  CLICK_EFFECT_DELETE_ALL_SCORE,
  CLICK_EFFECT_MAX
};

class ClickComponent : public Component
{
  bool b_state = false;
  SDL_Color active_color{ 255u, 255u, 255u, 255u };
  ClickEffect effect = CLICK_EFFECT_NONE;

public:
  ClickComponent () = default;
  ClickComponent (ClickEffect _effect, SDL_Color _active_color);
  ~ClickComponent () override = default;

  [[nodiscard]] bool GetState() const;
  [[nodiscard]] SDL_Color GetActiveColor() const;

  void UpdateComponent (App *app, Entity *owner) override;
};

#endif /* CLICK_MODULE_H */
