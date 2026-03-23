/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef ENTITY_MODULE_H
#define ENTITY_MODULE_H

#include <SDL3/SDL.h>
#include <algorithm>
#include <vector>

#include "component.h"

#include <memory>

class App;

class EntityFactory
{
public:
  EntityFactory () = default;
  ~EntityFactory () = default;

  Entity *MakeEntity ();
};

class Entity
{
public:
  Uint64 flag = 0u;
  std::vector<Component *> components{};

  Entity () = default;
  virtual ~Entity () = default;

  void AddComponent (Component *comp);
  template <typename T> [[nodiscard]] bool HasComponent () const;
  template <typename T> T *GetComponent ();

  virtual void Update (App *app);
};

template <typename T>
bool
Entity::HasComponent () const
{
  return std::any_of (components.begin (), components.end (),
                      [] (const auto &comp) {
                        return dynamic_cast<T> (comp.get ()) != nullptr;
                      });
}

template <typename T>
T *
Entity::GetComponent ()

{
  for (const auto &comp : components)
    {
      if (T *my_type = dynamic_cast<T *> (comp))
        {
          return my_type;
        }
    }
  return nullptr;
}

#endif /* ENTITY_MODULE_H */
