/* Copyright (c) 2026. LetTheMiceFree. */
#include "box.h"

BoxComponent::BoxComponent (bool _filled) : b_is_filled (_filled) {}

bool
BoxComponent::IsFilled () const
{
  return b_is_filled;
}

void
BoxComponent::UpdateComponent (App *app, Entity *owner)
{
}
