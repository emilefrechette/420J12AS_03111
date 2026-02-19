#include <cstdlib>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

#include <SDL3/SDL.h>

/*
 * Question 1)	Modifier la classe Entity pour supporter les patrons de conception de Composant et
 * UpdateMethod.
 * 1b) En commentaire, au dessus de la classe, expliquer pourquoi vous avez choisi cette
 * manière de représenter les composants (il existe plusieurs manières d'exprimer le patron).
 *
 * Répondez aux questions suivantes en exprimant vos solutions avec divers types de Composants:
 *
 * Question 2) Créer plusieurs entités et dessinez-les entités. Un simple rectangle suffit.
 *
 * Question 3) Détecter lorsque la souris chevauche sur une entité, changer son état.
 * IMPORTANT: Exprimé l'état sous forme de patron de conception.
 * Question 3b) Détecter lorsque la souris clique sur une entité et changer son état.
 *
 * Question 4) Changer la couleur de l'entité selon son état Non-chevauché, Chevauché, Cliqué
 * lorsque vient le temps de la dessiner.
 *
 * Question 5) Déplacer l'entité lorsque la souris est cliquée et se déplace simultanéement. Dans ce
 * cas, l'entité devrait se déplacer dans la même direction que la souris (style mécanique "drag and
 * drop").
 *
 * Question 6) Réorganiser la classe GameApp afin de respecter le patron de conception du Singleton.
 */

#include "entity.h"

static const std::string AppTitle = "Pratique d'examen - Mi-session";
static constexpr size_t MAX_SAMPLES = 100;

class GameApp final
{
	std::vector<float> FrameTimes;

  public:
	SDL_Window *Window = nullptr;
	SDL_Renderer *Renderer = nullptr;
	bool keyboard[SDL_SCANCODE_COUNT] = { false };
	std::vector<std::unique_ptr<Entity>> Entities;

	float SampleAverageFPS = 0.0f;

	GameApp ()
	{
		if (SDL_Init (SDL_INIT_VIDEO) == false)
			{
				SDL_LogCritical (1, "SDL failed to initialize! %s", SDL_GetError ());
				abort ();
			}
		Window = SDL_CreateWindow (AppTitle.c_str (), 800, 600, 0);
		if (Window == nullptr)
			{
				SDL_LogCritical (1, "SDL failed to create window! %s", SDL_GetError ());
				abort ();
			}
		Renderer = SDL_CreateRenderer (Window, nullptr);
		if (Renderer == nullptr)
			{
				SDL_LogCritical (1, "SDL failed to create renderer! %s", SDL_GetError ());
				abort ();
			}

		SDL_SetRenderVSync (Renderer, true);

		SDL_Time time;
		SDL_GetCurrentTime (&time);
		SDL_srand (time);

		for (int i = 0; i < 500; ++i)
			{
				auto e = std::make_unique<Entity> ();
				Entities.push_back (std::move (e));
			}
	}

	~GameApp ()
	{
		SDL_DestroyRenderer (Renderer);
		SDL_DestroyWindow (Window);
		SDL_Quit ();
	}

	void
	CalculateFPS (const float DeltaTime)
	{
		FrameTimes.push_back (DeltaTime);
		if (FrameTimes.size () > MAX_SAMPLES)
			{
				FrameTimes.erase (FrameTimes.begin ());
			}
		const float sum = std::accumulate (FrameTimes.begin (), FrameTimes.end (), 0.0f);
		const float average = sum / static_cast<float> (FrameTimes.size ());
		SampleAverageFPS = average > 0 ? 1.0f / average : 0;
	}
};

Sint32
main (int argc, char *argv[])
{
	std::unique_ptr<GameApp> app = std::make_unique<GameApp> ();
	bool running = true;
	uint64_t last_time = SDL_GetPerformanceCounter ();

	while (running == true)
		{
			SDL_Event event;
			while (SDL_PollEvent (&event) == true)
				{
					if (event.type == SDL_EVENT_QUIT)
						{
							running = false;
						}
					if (event.type == SDL_EVENT_KEY_DOWN)
						{
							app->keyboard[event.key.scancode] = true;
						}
					if (event.type == SDL_EVENT_KEY_UP)
						{
							app->keyboard[event.key.scancode] = false;
						}
				}

			const uint64_t freq = SDL_GetPerformanceFrequency ();
			const uint64_t current_time = SDL_GetPerformanceCounter ();
			const float delta_time
				= static_cast<float> (current_time - last_time) / static_cast<float> (freq);
			last_time = current_time;
			app->CalculateFPS (delta_time);

			SDL_SetRenderDrawColor (app->Renderer, 12, 12, 44, 255);
			SDL_RenderClear (app->Renderer);

			static float displayed;
			static float count;
			if (count <= 0.f)
				{
					displayed = app->SampleAverageFPS;
					count = 90.f;
				}
			else
				{
					count -= delta_time * 1000.f;
				}
			SDL_SetRenderDrawColor (app->Renderer, 255, 255, 255, 255);
			SDL_RenderDebugTextFormat (app->Renderer, 5, 5, "%.2f FPS", displayed);

			SDL_RenderPresent (app->Renderer);
		}
	return 0;
}
