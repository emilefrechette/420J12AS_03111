/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef CHARACTERS_MODULE_H
#define CHARACTERS_MODULE_H

#include "entity.h"

/**
 * @headerfile characters.h
 * @brief Représente différentes variations de l'entité de jeu de base dans lequel le patron de
 * conception du Update Method peut-être modulé.
 */

class Character_Rat : public Entity
{
  public:
	Character_Rat ();

	void ExecuteUpdate (float DeltaTime) override;
};

class Character_Punk : public Entity
{
  public:
	Character_Punk ();

	void ExecuteUpdate (float DeltaTime) override;
};

#endif /* CHARACTERS_MODULE_H */
