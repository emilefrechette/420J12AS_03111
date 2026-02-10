/*
 * Exemple de mise en place
 */

#include <SDL3/SDL.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

SDL_AppResult
SDL_AppInit (void **appstate, int argc, char *argv[])
{
	SDL_Log ("hello, world");

	SDL_WindowFlags win_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
	SDL_Window *win = SDL_CreateWindow ("Hello", 480, 480, win_flags);
	SDL_Renderer *rend = SDL_CreateRenderer(win, nullptr);

	*appstate = rend;

	return SDL_APP_CONTINUE;
}

SDL_AppResult
SDL_AppEvent (void *appstate, SDL_Event *event)
{
	switch (event->type)
		{
		case SDL_EVENT_QUIT:
			{
				return SDL_APP_SUCCESS;
			}
		case SDL_EVENT_KEY_DOWN:
			{
				SDL_Log ("Key pressed: '%s'", SDL_GetScancodeName (event->key.scancode));
				break;
			}
		default:
			{
				break;
			}
		}
	return SDL_APP_CONTINUE;
}

SDL_AppResult
SDL_AppIterate (void *appstate)
{
	SDL_Renderer *rend = (SDL_Renderer *)appstate;
	SDL_SetRenderDrawColor(rend, 200, 0, 125, 255);
	SDL_RenderClear(rend);
	SDL_RenderPresent(rend);

	return SDL_APP_CONTINUE;
}

void
SDL_AppQuit (void *appstate, SDL_AppResult result)
{
}

// Sint32
// main (int argc, char *argv[])
// {
// 	SDL_Log("hello, world");
// 	return 0;
// }
