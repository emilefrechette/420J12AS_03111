/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef ENTITY_MODULE_H
#define ENTITY_MODULE_H

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

/**
 * @brief Classe de base pour représenter une entité dans le jeu.
 */
class Entity
{
	/**
	 * @brief Représente le temps qui doit s'écouler avant le prochain "step" dans lequel l'entité
	 * exécutera son update.
	 */
	float StepCooldown = 0.f;

  public:
	Entity ();
	virtual ~Entity ();

	/**
	 * @brief Assigne la couleur de l'entité au moteur de rendu 'SDL Render 2D'.
	 * @param Renderer
	 */
	void SetRenderColorToEntity (SDL_Renderer *Renderer) const;

	/**
	 * @brief Crée le SDL_FRect représentant l'endroit ou dessiner l'entité.
	 * @return
	 */
	[[nodiscard]] SDL_FRect Destination () const;

	/**
	 * @brief Fonction principale représentant le patron de conception du Update Method.
	 * @param DeltaTime
	 */
	void Update (float DeltaTime);

	/**
	 * @brief Fonction interne pour implémenter les variations du Update Method.
	 * @details Parfois, on peut juste utiliser Update directement et la mettre virtual et omettre
	 * ExecuteUpdate. Dans ce cas, les entités partagent une logique commune dans Update alors que
	 * ExecuteUpdate représente la partie virtuelle.
	 * @param DeltaTime
	 */
	virtual void ExecuteUpdate (float DeltaTime) = 0;

  protected:
	/**
	 * @brief Position de l'entité dans le monde de jeu.
	 */
	SDL_FPoint Position;

	/**
	 * @brief Taille de l'entité dans le monde de jeu.
	 */
	SDL_FPoint Size;

	/**
	 * @brief Couleur et transparence de l'entité.
	 */
	SDL_Color Color;

	/**
	 * @brief Délai avant que l'entité prenne un prochain 'step' (appel sa fonction interne
	 * ExecuteUpdate).
	 */
	float DefaultStepCooldown = 90.f;
};

#endif /* ENTITY_MODULE_H */
