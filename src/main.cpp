/* Copyright (c) 2026. LetTheMiceFree. */

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <sqlite3.h>
#include <string>

#include "app.h"
#include "db.h"
#include "entity.h"

#include "components/box.h"
#include "components/render.h"
#include "components/text.h"
#include "components/transform.h"

// https://www.dafont.com/lazenby-computer.font?l[]=10

/**
 * Exemple du patron "poids mouche" ainsi que du patron "usine" (secondaire)
 * @param argc
 * @param argv
 * @return
 */
Sint32
main (Sint32 argc, char *argv[])
{
  SDL_Init (SDL_INIT_VIDEO);
  TTF_Init ();
  App *app = new App (std::string ("Database"), { 1280, 1280 });
  SDL_SetRenderVSync (app->GetRenderer (), true);
  app->now = SDL_GetPerformanceCounter ();

  app->LoadFont ();

  DatabaseClient *db = app->GetDatabaseClient ();
  db->OpenDatabase ();
  db->InsertScore ((ScoreRecord){ "Player1", 1500 });
  db->InsertScore ((ScoreRecord){ "Player2", 1200 });
  db->InsertScore ((ScoreRecord){ "Player3", 0 });
  db->InsertScore ((ScoreRecord){ "Player4", 3000 });

  std::vector<ScoreRecord> highscores;
  if (db->GetHighScores (highscores, 3) == SQLITE_OK)
    {
    }

  Entity *button = app->CreateEntity ();
  button->AddComponent (new BoxComponent ());
  button->AddComponent (
      new TransformComponent ({ 640 - 100, 640 - 40 }, { 200, 80 }));
  button->AddComponent (new RenderComponent (app->GetRenderer ()));

  Entity *score_text = app->CreateEntity ();
  score_text->AddComponent (new TextComponent (app, std::string ("Score: 0")));
  score_text->AddComponent (
      new TransformComponent ({ 320, 480 }, { 200, 80 }));
  score_text->AddComponent (new RenderComponent (app->GetRenderer ()));
  Entity *highscore_text = app->CreateEntity ();
  char str[255] = { 0 };
  SDL_snprintf (str, 255, "Highscore: %d", highscores[0].value);
  highscore_text->AddComponent (new TextComponent (app, str));
  highscore_text->AddComponent (
      new TransformComponent ({ 960 - 220, 480 }, { 200, 80 }));
  highscore_text->AddComponent (new RenderComponent (app->GetRenderer ()));

  // Mise-à-jour du programme
  while (app->IsGameOver () == false)
    {
      SDL_Event e = {};
      app->last = app->now;
      app->now = SDL_GetPerformanceCounter ();
      Uint64 span = app->now - app->last;
      app->dt_ms = static_cast<float> (span)
                   / static_cast<float> (SDL_GetPerformanceFrequency ());

      while (SDL_PollEvent (&e) == true)
        {
          //SDL_Log ("Event: %u", e.type);
          switch (e.type)
            {
            default:
              {
                break;
              }
            case SDL_EVENT_QUIT:
              {
                app->SetGameOver (true);
                break;
              }
            }
        }

      SDL_SetRenderDrawColor (app->GetRenderer (), 0, 55, 0, 255);
      SDL_RenderClear (app->GetRenderer ());

      for (const auto &ent : app->GetEntities ())
        {
          ent->Update (app);
        }

      SDL_RenderPresent (app->GetRenderer ());
    }
  return 0;
}
