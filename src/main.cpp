/* Copyright (c) 2026. LetTheMiceFree. */

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "app.h"
#include "entity.h"

#include "components/box.h"
#include "components/render.h"
#include "components/sprite.h"
#include "components/transform.h"


/**
 * Exemple du patron "poids mouche" ainsi que du patron "usine" (secondaire)
 * @param argc
 * @param argv
 * @return
 */
Sint32
main (Sint32 argc, char *argv[])
{
  // Initialization
  SDL_Init (SDL_INIT_VIDEO);
  App *app = new App ("Flyweight", { 1280, 1280 });
  SDL_SetRenderVSync (app->GetRenderer (), true);
  app->now = SDL_GetPerformanceCounter ();

  app->LoadSpriteResources ();

  /* Test des patrons: */
  for (Sint32 i = 0; i < 10; i++)
    {
      Entity *entity = app->CreateEntity ();
      entity->AddComponent (new TransformComponent (
          { static_cast<float> (100 + SDL_rand (1000)),
            static_cast<float> (100 + SDL_rand (1000)) },
          { 200.f, 200.f }));
      entity->AddComponent (new BoxComponent ());
      entity->AddComponent (new RenderComponent (app->GetRenderer ()));
      /** @note Le "poids mouche" est implémenter pour le Sprite. */
      entity->AddComponent (new SpriteComponent (app,
                                                 std::string ("res/01.png")));
    }

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
          SDL_Log ("Event: %u", e.type);
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
