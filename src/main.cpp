#include <cstdlib>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

#include <SDL3/SDL.h>

/*
 *
 * Consignes de remises) Créez un nouveau répertoire git et soumettez-le sur votre GitHub personnel.
 * Chaque question devrait être un COMMIT individuel, identifié par le numéro de la question. 10%
 *
 * Tous les outils traditionnels sont permis pour l'examen: recherche, forum de discussion, exemples
 * du cours, etc. L'IA est permis, mais just Copilot: https://copilot.microsoft.com/ et les IA
 * intégrées dans les IDEs. Pour copilot, s.v.p. ajoutez en commentaire le ou les lien(s) de votre
 * ou vos conversation(s) reliée(s) à l'examen (bouton en haut à droite). Vous pouvez être accusé de
 * plagiat autrement. De plus, il est interdit de demander verbatim les questions de l'examen dans
 * les conversations de copilot.
 *
 * Finalement, il n'est pas permis d'échanger des informations entre les étudiants (tolérance zéro)
 * ou de converser avec d'autres individus durant l'examen.
 *
 * - - - - -
 *
 * Question 1)	Modifiez les classes GameApp et Entity de sorte à supporter les patrons de
 * conception de Composant et UpdateMethod. 20%
 *
 * Question 2) Créez les composants nécessaires afin de pouvoir dessiner les entités sous formes de
 * rectangles de diverse grandeur, position et couleur. 20%
 *
 * Question 3) Créez deux types d'entités: les Jetons (Tokens) et le Sac à dos (Bag).
 * - Jetons : Créez 10 à 20 jetons dans la moitié supérieure de l'écran. La taille des jetons est de
 * 40x40. La position des jetons est aléatoire. Le pointage des jetons doit être représenté par
 * un patron de conception d'État, dont les trois états sont: Jeton normal (1 points), Super-jeton
 * (3 points), Omega Jeton (10 points).
 * - Sac à Dos: Créez le sac à dos au milieu de la moitié supérieure de l'écran. La taille du sac à
 * dos est de 120x150. 20%
 *
 * Question 4) Créez les composants nécessaires afin de supporter la mécanique de "glisser-déposer"
 * sur les jetons. Avec la souris, on doit pouvoir cliquer sur un jeton et le déplacer. Représenter
 * l'état du jeton durant l'action de "glisser-déposer" sous former de patron de conception d'État
 * comportant les états: Non-Sélectionné, Sélectionné et Cliqué. 15%
 *
 * Question 5) Créez les composants nécessaires afin de supporter la mécanique de collecte des
 * jetons sur le sac à dos. Lorsqu'un jeton est déposé sur le sac à dos, il faut l'enlever de
 * l'affichage ou le détruire, et ajouter le pointage correspondant dans le sac à dos.
 * (Affichez le nombre de points à l'écran avec SDL_RenderDebugTextFormat.) 15%
 *
 *
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

			SDL_Point win_size = { 0 };
			SDL_GetWindowSize (app->Window, &win_size.x, &win_size.y);

			SDL_SetRenderDrawColor (app->Renderer, 42, 42, 104, 255);
			SDL_RenderLine (app->Renderer, 0.f, 0.f, static_cast<float> (win_size.x), static_cast<float> (win_size.y));
			SDL_RenderLine (app->Renderer, 0.f, static_cast<float> (win_size.y), static_cast<float> (win_size.x), 0.f);

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
