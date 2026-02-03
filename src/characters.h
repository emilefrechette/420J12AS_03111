/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef CHARACTERS_MODULE_H
#define CHARACTERS_MODULE_H

#include "entity.h"

/**
 * @headerfile characters.h
 * @brief Variations d'entités avec certains composants propres.
 */

class Character_Rat : public Entity
{
  public:
	Character_Rat ();
};

class Character_Punk : public Entity
{
  public:
	Character_Punk ();
};

#endif /* CHARACTERS_MODULE_H */
