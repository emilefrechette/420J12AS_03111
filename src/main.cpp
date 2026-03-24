/* Copyright (c) 2026. LetTheMiceFree. */

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

#include "app.h"
#include "db.h"

// https://www.dafont.com/lazenby-computer.font?l[]=10

/**
 * Exemple de base de donnée
 * @param argc
 * @param argv
 * @return
 */
Sint32
main (Sint32 argc, char *argv[])
{
  SDL_Init (SDL_INIT_VIDEO);
  TTF_Init ();
  App *app = new App ("Database", { 1280, 1280 }, "Me");
  SDL_SetRenderVSync (app->GetRenderer (), true);
  app->now = SDL_GetPerformanceCounter ();

  app->LoadFont ();
  app->CreateScene();

  DatabaseClient *db = app->GetDatabaseClient ();
  if (db == nullptr)
    {
      SDL_LogError(1, "There was an issue with the database!");
      return 1;
    }
  db->OpenDatabase ();

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
          // SDL_Log ("Event: %u", e.type);
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
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
              {
                if (e.button.button == 1)
                  {
                    app->RegisterDownLMB ();
                  }
                break;
              }
            }
        }
      app->Update ();
    }
  return 0;
}
