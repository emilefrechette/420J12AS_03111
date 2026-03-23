/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef RENDER_MODULE_H
#define RENDER_MODULE_H

#include <SDL3/SDL.h>

#include "component.h"

class RenderComponent : public Component
{
  SDL_Renderer *renderer;

public:
  RenderComponent (SDL_Renderer *_renderer );
  ~RenderComponent () override = default;

  void UpdateComponent (App *app, Entity *owner) override;
};

#endif /* RENDER_MODULE_H */
