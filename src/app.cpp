/* Copyright (c) 2026. LetTheMiceFree. */
#include "app.h"

#include <algorithm>

#include "components/box.h"
#include "components/click.h"
#include "components/color.h"
#include "components/hover.h"
#include "components/render.h"
#include "components/text.h"
#include "components/transform.h"
#include "db.h"
#include "entity.h"

App::App (const std::string &title, const SDL_Point &dims,
          const std::string &player)
{
  window = SDL_CreateWindow (title.c_str (), dims.x, dims.y, 0);
  renderer = SDL_CreateRenderer (window, nullptr);
  text_engine = TTF_CreateRendererTextEngine (renderer);
  db_client = new DatabaseClient ();
  entity_factory = new EntityFactory ();
  local_score.player_name = player;
}

void
App::Update ()
{
  SDL_GetMouseState (&mouse_pos.x, &mouse_pos.y);
  SDL_SetRenderDrawColor (renderer, 0, 55, 0, 255);
  SDL_RenderClear (renderer);

  for (const auto &ent : entities)
    {
      ent->Update (this);
    }

  SDL_RenderPresent (renderer);
  b_is_lmb_down = false;
}

void
App::LoadFont ()
{
  font = TTF_OpenFont ("res/LazenbyCompLiquid.ttf", 40);
  if (font == nullptr)
    {
      SDL_LogError (1, "Failed to load font! %s", SDL_GetError ());
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
SDL_FPoint
App::GetMousePosition () const
{
  return mouse_pos;
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

bool
App::IsDownLMB () const
{
  return b_is_lmb_down;
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

void
App::RegisterDownLMB ()
{
  b_is_lmb_down = true;
}

void
App::SetScore (Sint32 s)
{
  local_score.value = s;
  auto *text = score_text->GetComponent<TextComponent> ();
  char content[255]{};
  SDL_snprintf (content, 255, "Score: %d", local_score.value);
  text->UpdateGlyph (this, content);
}

void
App::IncrementScore ()
{
  local_score.value++;
  auto *text = score_text->GetComponent<TextComponent> ();
  char content[255]{};
  SDL_snprintf (content, 255, "Score: %d", local_score.value);
  text->UpdateGlyph (this, content);
}

void
App::PostScore ()
{
  db_client->InsertScore (local_score);
  SetScore (0);
}

void
App::GetHighscore ()
{
  std::vector<ScoreRecord> highscores;
  if (db_client->GetHighScores (highscores, 3) == SQLITE_OK && !highscores.empty ())
    {
      auto *text = highscore_text->GetComponent<TextComponent> ();
      char content[255]{};
      SDL_snprintf (content, 255, "Highscore: %d", highscores[0].value);
      text->UpdateGlyph (this, content);
    }
  else
    {
      auto *text = highscore_text->GetComponent<TextComponent> ();
      char content[255]{};
      SDL_snprintf (content, 255, "Highscore:");
      text->UpdateGlyph (this, content);
    }
}
void
App::DeleteScores ()
{
  db_client->DeleteAllScores ();

  auto *text1 = score_text->GetComponent<TextComponent> ();
  char content1[255]{};
  SDL_snprintf (content1, 255, "Score: 0");
  text1->UpdateGlyph (this, content1);

  auto *text2 = highscore_text->GetComponent<TextComponent> ();
  char content2[255]{};
  SDL_snprintf (content2, 255, "Highscore:");
  text2->UpdateGlyph (this, content2);
}

void
App::CreateScene ()
{
  const ClickEffect effects[4] = {
    CLICK_EFFECT_INC_SCORE,
    CLICK_EFFECT_SEND_SCORE,
    CLICK_EFFECT_GET_HIGHSCORE,
    CLICK_EFFECT_DELETE_ALL_SCORE,
  };
  const Entity *handles[4] = {
    inc_button,
    post_button,
    hi_score_button,
    delete_button,
  };
  for (Sint32 i = 0; i < 4; i++)
    {
      Entity *button = CreateEntity ();
      button->AddComponent (new BoxComponent (true));
      button->AddComponent (
          new ClickComponent (effects[i], { 255u, 0, 0u, 255u }));
      button->AddComponent (new ColorComponent ({ 200u, 122u, 0u, 255u }));
      button->AddComponent (new HoverComponent ({ 255u, 255u, 0u, 255u }));
      button->AddComponent (new TransformComponent (
          { 640 - 100, 640 - 40 + 120 * static_cast<float> (i) },
          { 200, 80 }));
      button->AddComponent (new RenderComponent (GetRenderer ()));
      handles[i] = button;
    }

  Entity *text1 = CreateEntity ();
  text1->AddComponent (new TextComponent (this, std::string ("Score: 0")));
  text1->AddComponent (new TransformComponent ({ 320, 480 }, { 200, 80 }));
  text1->AddComponent (new RenderComponent (GetRenderer ()));
  score_text = text1;

  Entity *text2 = CreateEntity ();
  text2->AddComponent (new TextComponent (this, "Highscore:"));
  text2->AddComponent (
      new TransformComponent ({ 960 - 220, 480 }, { 200, 80 }));
  text2->AddComponent (new RenderComponent (GetRenderer ()));
  highscore_text = text2;
}
