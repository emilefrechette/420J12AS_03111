/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef BOX_MODULE_H
#define BOX_MODULE_H

#include "component.h"

class BoxComponent : public Component
{
  bool b_is_filled = false;

public:
  BoxComponent () = default;
  BoxComponent (bool _filled);
  ~BoxComponent () override = default;

  [[nodiscard]] bool IsFilled() const;
  void UpdateComponent (App *app, Entity *owner) override;
};

#endif /* BOX_MODULE_H */
