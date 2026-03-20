/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef APP_MODULE_H
#define APP_MODULE_H

#include <SDL3/SDL.h>
#include <memory>
#include <vector>

class EntityFactory;
class SpriteResourceFactory;
class Entity;

class App final
{
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  EntityFactory * entity_factory;
  SpriteResourceFactory * sprite_resource_factory;

  std::vector<Entity*> entities = std::vector<Entity*> ();

  bool b_game_over = false;

public:
  Uint64 now = 0u;
  Uint64 last = 0u;
  float fps = 0.f;
  float dt_ms = 0.f;

  App (const std::string &title, const SDL_Point &dims);
  ~App () = default;

  void LoadSpriteResources () const;
  [[nodiscard]] SDL_Renderer *GetRenderer() const;
  void SetGameOver(bool b_state);
  [[nodiscard]] bool IsGameOver() const;
  [[nodiscard]] Entity * CreateEntity();
  void DeleteEntity (Entity *ent);
  std::vector<Entity *> &GetEntities();
};
#endif /* APP_MODULE_H */
