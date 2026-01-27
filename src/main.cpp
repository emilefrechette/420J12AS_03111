/*
 * Patron de conception: Update Method
 *
 */

#include "characters.h"
#include "entity.h"

#include <SDL3/SDL.h>
#include <memory>
#include <numeric>
#include <string>

#include <vector>

static const std::string AppTitle = "Patrons de conception: Update Method";

/**
 * @brief Échantillon maximum de durées des images, à utiliser pour calculer la moyenne du
 * nombre d'images par seconde.
 */
static constexpr size_t MAX_SAMPLES = 100;

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
	 * 2D'. Il existe d'autres moteurs de rendu possible qui peuvent être utilisés. Ces derniers
	 * sont plus complexes.
	 */
	SDL_Renderer *Renderer = nullptr;

	/**
	 * @brief Échantillon de durées des images. Un vecteur (collection dynamique) de float est
	 * utilisée. Sa taille est limitée manuelle par la limite de MAX_SAMPLES. La collection contient
	 * une séquence de durées des images utilisées dans le prochain calcul de la durée moyenne de
	 * l'échantillon d'images par seconde.
	 */
	std::vector<float> FrameTimes;

	/**
	 * @brief Nombre d'images par secondes de l'échantillon d'images.
	 */
	float SampleAverageFPS = 0.0f;

	/**
	 * @brief Collection contenant toutes les entités de jeu.
	 */
	std::vector<std::unique_ptr<Entity>> Entities;

  public:
	/**
	 * Constructeur de l'application.
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
		SDL_SetDefaultTextureScaleMode (Renderer, SDL_SCALEMODE_NEAREST);

		Entities = std::vector<std::unique_ptr<Entity>> ();
		Entities.push_back (std::make_unique<Character_Rat> ());
		Entities.push_back (std::make_unique<Character_Rat> ());
		Entities.push_back (std::make_unique<Character_Rat> ());
		Entities.push_back (std::make_unique<Character_Punk> ());
		Entities.push_back (std::make_unique<Character_Punk> ());
		Entities.push_back (std::make_unique<Character_Punk> ());
	}

	/**
	 * @brief Destructeur de la classe principale de l'application. S'exécute lorsque l'application
	 * se termine, libérant les ressources SDL. Cette étape est souvent inutile sur plusieurs
	 * sytèmes d'exploitation, mais dans certains cas, elle peut s'avérer conséquente.
	 */
	~GameApp ()
	{
		SDL_DestroyRenderer (Renderer);
		SDL_DestroyWindow (Window);
		SDL_Quit ();
	}

	/**
	 * @brief Fonction de calcul de la durée moyenne de l'échantillon de durées d'image qui se
	 * retrouve dans tous les exemples.
	 * @param DeltaTime
	 */
	void
	CalculateFPS (const float DeltaTime)
	{
		/* Insertion de la durée de la dernière image. */
		FrameTimes.push_back (DeltaTime);
		/* Si l'échantillon dépasse la taille maximum avec l'ajout de cette nouvelle image, retirer
		 * la première image. */
		if (FrameTimes.size () > MAX_SAMPLES)
			{
				FrameTimes.erase (FrameTimes.begin ());
			}
		/* std::accumulate permet de faire automatiquement la somme de l'échantillon. */
		const float sum = std::accumulate (FrameTimes.begin (), FrameTimes.end (), 0.0f);

		/* Une moyenne est calculée en divisant la somme des durées par le nombre totale de durées
		 * dans l'échantillon. */
		const float average = sum / static_cast<float> (FrameTimes.size ());

		/* Calculer le débit moyen d'images par seconde à partir de la moyenne de durées de
		 * l'échantillon d'images, en évitant la division par zéro. */
		SampleAverageFPS = (average > 0) ? 1.0f / average : 0;
	}

	/**
	 * @brief Méthode principale qui "capture" le déroulement du programme dans le patron de
	 * conception de la boucle de jeu. De plus, gère aussi la boucle d'événements de même que tout
	 * le reste du jeu.
	 * @details Endroit principal où construire la logique "macro" du programme.
	 */
	void
	Run ()
	{
		bool running = true;
		uint64_t last_time = SDL_GetPerformanceCounter ();
		/* Patron de conception: Boucle de jeu. */
		while (running == true)
			{
				/* Patron de conception: Boucle d'événements (on ne fait pas grand chose dans cet
				 * example ici). */
				SDL_Event event;
				while (SDL_PollEvent (&event) == true)
					{
						if (event.type == SDL_EVENT_QUIT)
							running = false;
					}

				/* Calcul de la durée de l'image (itération de la boucle de jeu) précédente. */
				const uint64_t freq = SDL_GetPerformanceFrequency ();
				const uint64_t current_time = SDL_GetPerformanceCounter ();
				const float delta_time
					= static_cast<float> (current_time - last_time) / static_cast<float> (freq);
				last_time = current_time;
				CalculateFPS (delta_time);

				/* Patron de conception: Update Method */
				for (const auto &ent : Entities)
					{
						ent->Update (delta_time);
					}

				/* Rendu simplifié. */
				SDL_SetRenderDrawColor (Renderer, 12, 12, 44, 255);
				SDL_RenderClear (Renderer);
				for (const auto &ent : Entities)
					{
						ent->SetRenderColorToEntity (Renderer);
						SDL_FRect dst = ent->Destination ();
						SDL_RenderFillRect (Renderer, &dst);
					}
				SDL_RenderPresent (Renderer);
			}
	}
};

/**
 * @brief Point d'entrée du programme.
 * @param argc
 * @param argv
 * @return
 */
Sint32
main (int argc, char *argv[])
{
	GameApp app;
	app.Run ();
	return 0;
}
