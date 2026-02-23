/* Copyright (c) 2026. LetTheMiceFree. */
#include "entity.h"

#include "component.h"

Entity::~Entity ()
{
	for (const auto &comp : components)
		{
			delete comp;
		}
	components.clear ();
}

void
Entity::Update (GameApp *app)
{

	for (const auto &comp : components)
		{
			comp->UpdateComponent (app, this);
		}
}
