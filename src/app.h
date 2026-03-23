/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef APP_MODULE_H
#define APP_MODULE_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>

class DatabaseClient;
class EntityFactory;
class Entity;

class App final
{
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  TTF_TextEngine *text_engine = nullptr;

  DatabaseClient *db_client = nullptr;

  EntityFactory * entity_factory;
  std::vector<Entity*> entities = std::vector<Entity*> ();
  TTF_Font *font = nullptr;

  Entity *button;
  Entity *score;
  Entity *highscore;

  bool b_game_over = false;

public:
  Uint64 now = 0u;
  Uint64 last = 0u;
  float fps = 0.f;
  float dt_ms = 0.f;

  App (const std::string &title, const SDL_Point &dims);
  ~App () = default;

  void LoadFont();

  [[nodiscard]] SDL_Renderer *GetRenderer() const;
  [[nodiscard]] TTF_TextEngine *GetTextEngine() const;
  [[nodiscard]] TTF_Font *GetFont() const;
  [[nodiscard]] DatabaseClient *GetDatabaseClient () const;
  void SetGameOver(bool b_state);
  [[nodiscard]] bool IsGameOver() const;
  [[nodiscard]] Entity * CreateEntity();
  void DeleteEntity (Entity *ent);
  std::vector<Entity *> &GetEntities();
};

#endif /* APP_MODULE_H */
