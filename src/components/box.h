/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef BOX_MODULE_H
#define BOX_MODULE_H

#include "component.h"

class BoxComponent : public Component
{
public:
  BoxComponent () = default;
  ~BoxComponent () override = default;

  void UpdateComponent (App *app, Entity *owner) override;
};

#endif /* BOX_MODULE_H */
