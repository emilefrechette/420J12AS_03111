/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef ENTITY_MODULE_H
#define ENTITY_MODULE_H

#include "components.h"

#include <SDL3/SDL_render.h>

/**
 * @brief Classe de base pour représenter une entité dans le jeu. Conteneur de composants.
 */
class Entity
{
	/**
	 * @brief Un séquence binaire (liste de 64 zéros) dans un Uint64.
	 * Chaque chiffre dans la séquence indique si l'entité détient ou non
	 * un composant. 0 = pas de composant, 1 = composant.
	 * @note Le code binaire de chaque composant est dans le header components.h
	 */
	Uint64 flag = 0u;
	/* Exemple: si le flag est 0b0111 0000 0000 0000... les composants présent sont
	 * magic, movement et render.
	 */

  public:
	Entity () = default;
	virtual ~Entity () = default;

	/* Les composants (toujours présent ; donc forme de gaspillage de mémoire). */
	HealthComponent health;
	MagicComponent magic;
	MovementComponent movement;
	RenderComponent render;
	TransformComponent transform;

	/**
	 * @brief Rajouter un composant ou plusieurs composants au flag.
	 */
	void AddComponent (Uint64 comps);
	/**
	 * @brief Regarder si un composant ou plusieurs composants sont présent.
	 * @param comps
	 * @return
	 */
	[[nodiscard]] bool HasComponent (Uint64 comps) const;

	void RemoveComponent (Uint64 comps);

	/* Exemples simples de pseudo-systèmes. */

	virtual void MovementUpdate (float DeltaTime);
	void RenderUpdate (SDL_Renderer *renderer);
};

#endif /* ENTITY_MODULE_H */
