/*
 * Patrons de conception: Singleton et Observateur
 */

#include "entity.h"

#include <SDL3/SDL.h>
#include <cstdlib>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

class IObserver;

/**
 * @brief Nombre max. de frame à analyser pour calculer le delta time/FPS.
 */
static constexpr size_t MAX_SAMPLES = 100;

static const std::string AppTitle = "Patrons de conception: Singleton et Observateur";

/**
 * @brief Classe principale de l'application. Sert à orchestrer SDL et les autres potentielles
 * bibliothèques, ainsi que les objets principaux du programme.
 */
class GameApp final /* La classe est final, aucune classe dérivée ne peut être créée. */
{
	/**
	 * @brief Pointeur vers la ressource SDL représentant la fenêtre d'application.
	 */
	SDL_Window *Window = nullptr;
	/**
	 * @brief Pointeur vers la ressource SDL représentant le moteur de rendu de type 'SDL_Render
	 * 2D'.
	 */
	SDL_Renderer *Renderer = nullptr;

	/**
	 * @brief Échantillon de durées des images.
	 */
	std::vector<float> FrameTimes;

	/**
	 * @brief Nombre d'images par secondes de l'échantillon d'images.
	 */
	float SampleAverageFPS = 0.0f;

	bool keyboard[SDL_SCANCODE_COUNT] = { false };

	/**
	 * @brief PATRON OBSERVATEUR : Liste des abonnés.
	 * Le "Sujet" (GameApp) maintient une liste de pointeurs vers les observateurs.
	 */
	std::vector<IObserver *> Observers;

	/**
	 * @brief Collection d'entités pour l'UpdateMethod
	 */
	std::vector<std::unique_ptr<Entity>> Entities;

	/**
	 * @brief PATRON SINGLETON - CONSTRUCTEUR PRIVÉ : Empêche l'instanciation directe de la classe.
	 */
	GameApp ()
	{
		/* Initialization de la bibliothèque, la fenêtre principale, et le moteur de rendu
		 * 'SDL_Render 2D'. */
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

		SDL_Time time;
		SDL_GetCurrentTime (&time);
		SDL_srand (time);

		for (int i = 0; i < 500; ++i)
			{
				auto e = std::make_unique<Entity> ((float)SDL_rand (800), (float)SDL_rand (600));
				AddObserver (e.get ());
				Entities.push_back (std::move (e));
			}
	}

	/**
	 * @brief PATRON SINGLETON - DESTRUCTEUR PRIVÉ : Assure que seule la classe elle-même gère sa
	 * fin de vie.
	 */
	~GameApp ()
	{
		SDL_DestroyRenderer (Renderer);
		SDL_DestroyWindow (Window);
		SDL_Quit ();
	}

  public:
	/**
	 * @brief PATRON SINGLETON - SUPPRESSION DE LA COPIE : Un singleton ne doit jamais être copié ou
	 * déplacé.
	 */
	GameApp (const GameApp &) = delete;
	GameApp &operator= (const GameApp &) = delete;
	GameApp (GameApp &&) = delete;
	GameApp &operator= (GameApp &&) = delete;

	/**
	 * @brief PATRON SINGLETON -  Point d'accès statique pour obtenir l'instance unique du
	 * Singleton. Utilise le "Singleton de Meyers".
	 */
	static GameApp &
	GetInstance ()
	{
		static GameApp instance; /* Créé une seule fois, thread-safe depuis C++11 */
		return instance;
	}

	/**
	 * @brief PATRON DE L'OBSERVATEUR - Permet d'ajouter un observateur à la liste de notification.
	 */
	void
	AddObserver (IObserver *obs)
	{
		Observers.push_back (obs);
	}

	/**
	 * @brief PATRON DE L'OBSERVATEUR - Notifie tous les observateurs enregistrés d'un changement.
	 */
	void
	Notify () const
	{
		for (const auto obs : Observers)
			{
				obs->OnNotify ();
			}
	}

	/**
	 * @brief Fonction de calcul de la durée moyenne de l'échantillon de durées d'image.
	 */
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

	/**
	 * @brief Méthode principale qui "capture" le déroulement du programme.
	 */
	void
	Run ()
	{
		bool running = true;
		uint64_t last_time = SDL_GetPerformanceCounter ();

		/* Patron de conception: Boucle de jeu. */
		while (running == true)
			{
				SDL_Event event;
				/* La fonction SDL_PollEvent est une implémentation-type du patron de conception de
				 * la File d'attente d'événements. */
				while (SDL_PollEvent (&event) == true)
					{
						if (event.type == SDL_EVENT_QUIT)
							{
								running = false;
							}
						if (event.type == SDL_EVENT_KEY_DOWN)
							{
								keyboard[event.key.scancode] = true;
								/* NOTIFICATION : On prévient les observateurs qu'une touche a été
								 * pressée. */
								Notify ();
							}
						if (event.type == SDL_EVENT_KEY_UP)
							{
								keyboard[event.key.scancode] = false;
							}
					}

				/* Calcul de la durée de l'image précédente. */
				const uint64_t freq = SDL_GetPerformanceFrequency ();
				const uint64_t current_time = SDL_GetPerformanceCounter ();
				const float delta_time
					= static_cast<float> (current_time - last_time) / static_cast<float> (freq);
				last_time = current_time;
				CalculateFPS (delta_time);

				/* Update Method sur la collection. */
				for (auto &entity : Entities)
					{
						entity->Update (delta_time);
					}

				/* Rendu simplifié. */
				SDL_SetRenderDrawColor (Renderer, 12, 12, 44, 255);
				SDL_RenderClear (Renderer);

				for (auto &entity : Entities)
					{
						entity->Draw (Renderer);
					}

				SDL_RenderPresent (Renderer);
			}
	}
};

/**
 * @brief Point d'entrée du programme.
 */
Sint32
main (int argc, char *argv[])
{
	/* On récupère l'instance unique via GetInstance() au lieu de déclarer une variable locale. */
	GameApp::GetInstance ().Run ();
	return 0;
}
