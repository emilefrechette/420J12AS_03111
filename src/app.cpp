/* Copyright (c) 2026. LetTheMiceFree. */
#include "app.h"

#include <algorithm>

#include "components/sprite.h"
#include "entity.h"

App::App (const std::string &title, const SDL_Point &dims)
{
  window = SDL_CreateWindow (title.c_str (), dims.x, dims.y, 0);
  renderer = SDL_CreateRenderer (window, nullptr);
  entity_factory = new EntityFactory ();
  sprite_resource_factory = new SpriteResourceFactory ();
}

void
App::LoadSpriteResources () const
{
  sprite_resource_factory->MakeSprite (renderer, "res/01.png");
  sprite_resource_factory->MakeSprite (renderer, "res/02.png");
  sprite_resource_factory->MakeSprite (renderer, "res/03.png");
  sprite_resource_factory->MakeSprite (renderer, "res/04.png");
}

SDL_Renderer *
App::GetRenderer () const
{
  return renderer;
}

void
App::SetGameOver (bool b_state)
{
  b_game_over = b_state;
}

bool
App::IsGameOver () const
{
  return b_game_over;
}

Entity *
App::CreateEntity ()
{
  Entity *entity = entity_factory->MakeEntity ();
  entities.push_back (entity);
  return entity;
}

void

App::DeleteEntity (Entity *ent)

{
  if (ent == nullptr)
    {
      return;
    }
  auto it = std::find (entities.begin (), entities.end (), ent);
  if (it != entities.end ())
    {
      entities.erase (it);
      delete ent;
    }
}
std::vector<Entity *> &
App::GetEntities ()
{
  return entities;
}
