/* Copyright (c) 2026. LetTheMiceFree. */

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <steam/steam_api.h>
#include <steam/steam_api_flat.h>
#include <string>

#include "app.h"
#include "entity.h"

/**
 * Exemple de linkage avec Steamworks
 * @param argc
 * @param argv
 * @return
 */
Sint32
main (Sint32 argc, char *argv[])
{
  // if (SteamAPI_RestartAppIfNecessary(480)) {
  //     return 1;
  // }
  if (!SteamAPI_Init ())
    {
      SDL_Log ("Steam API failed to initialize");
      return -1;
    }
  uint32_t appId = SteamUtils ()->GetAppID ();
  SDL_Log ("Running with Steam AppID: %u", appId);
  SDL_Init (SDL_INIT_VIDEO);
  TTF_Init ();
  App *app = new App ("Steamworks Linkage", { 960, 960 });
  SDL_SetRenderVSync (app->GetRenderer (), true);
  app->now = SDL_GetPerformanceCounter ();

  if (SteamUser () && SteamUser ()->BLoggedOn ())
    {
      SDL_Log ("SteamUser available");
      ISteamFriends *friends = SteamFriends ();
      if (friends)
        {
          SDL_Log ("Player name: %s", friends->GetPersonaName ());
        }
      CSteamID steamID = SteamAPI_ISteamUser_GetSteamID(SteamUser());
      if (steamID.IsValid ())
        {
          SDL_Log("Steam ID: %llu", steamID.ConvertToUint64());
        }
    }
  else
    {
      SDL_Log ("Steam user not logged in or session invalid");
    }

  if (SteamUtils ())
    {
      SDL_Log ("SteamUtils available");
      SDL_Log ("App ID: %u", SteamUtils ()->GetAppID ());
    }

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
            }
        }
      app->Update ();
    }
  SteamAPI_Shutdown ();
  return 0;
}
