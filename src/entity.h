/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef ENTITY_MODULE_H
#define ENTITY_MODULE_H

#include <vector>

class GameApp;
class Component;

class Entity
{
  public:
	std::vector<Component *> components;

	Entity () = default;
	virtual ~Entity ();

	template <typename T>
	T *
	GetComponentIfExists () const
	{
		for (Component *c : components)
			{
				if (auto *casted = dynamic_cast<T *> (c))
					return casted;
			}
		return nullptr;
	}

	virtual void Update (GameApp *app);
};

#endif /* ENTITY_MODULE_H */
