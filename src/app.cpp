/* Copyright (c) 2026. LetTheMiceFree. */
#include "app.h"

#include <algorithm>

#include "db.h"
#include "entity.h"

App::App (const std::string &title, const SDL_Point &dims)
{
  window = SDL_CreateWindow (title.c_str (), dims.x, dims.y, 0);
  renderer = SDL_CreateRenderer (window, nullptr);
  text_engine = TTF_CreateRendererTextEngine (renderer);
  db_client = new DatabaseClient();
  entity_factory = new EntityFactory ();
}

void
App::LoadFont ()
{
  font = TTF_OpenFont ("res/LazenbyCompLiquid.ttf", 40);
  if (font == nullptr)
    {
      SDL_LogError(1, "Failed to load font! %s", SDL_GetError());
    }
}

SDL_Renderer *
App::GetRenderer () const
{
  return renderer;
}

TTF_TextEngine *
App::GetTextEngine () const
{
  return text_engine;
}

TTF_Font *
App::GetFont () const
{
  return font;
}

DatabaseClient *
App::GetDatabaseClient () const
{
  return db_client;
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
