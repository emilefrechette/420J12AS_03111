/* Copyright (c) 2026. LetTheMiceFree. */
#include "entity.h"

Entity *
EntityFactory::MakeEntity ()
{
  auto entity = new Entity ();
  return entity;
}

void
Entity::AddComponent (Component *comp)
{
  components.push_back (comp);
}

void
Entity::Update (App *app)
{
  for (const auto &comp : components)
    {
      comp->UpdateComponent (app, this);
    }
}
