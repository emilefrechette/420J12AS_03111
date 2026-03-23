/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef APP_MODULE_H
#define APP_MODULE_H

#include "db.h"

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
  ScoreRecord local_score{ "", 0 };

  DatabaseClient *db_client = nullptr;

  SDL_FPoint mouse_pos{};
  bool b_is_lmb_down = false;

  EntityFactory *entity_factory;
  std::vector<Entity *> entities = std::vector<Entity *> ();
  TTF_Font *font = nullptr;

  bool b_game_over = false;

public:
  Uint64 now = 0u;
  Uint64 last = 0u;
  float fps = 0.f;
  float dt_ms = 0.f;

  Entity *inc_button = nullptr;
  Entity *post_button = nullptr;
  Entity *hi_score_button = nullptr;
  Entity *delete_button = nullptr;
  Entity *score_text = nullptr;
  Entity *highscore_text = nullptr;

  App (const std::string &title, const SDL_Point &dims,
       const std::string &player);
  ~App () = default;

  void Update ();

  void LoadFont ();

  void CreateScene ();
  [[nodiscard]] SDL_Renderer *GetRenderer () const;
  [[nodiscard]] TTF_TextEngine *GetTextEngine () const;
  [[nodiscard]] TTF_Font *GetFont () const;
  [[nodiscard]] DatabaseClient *GetDatabaseClient () const;
  [[nodiscard]] SDL_FPoint GetMousePosition () const;
  void SetGameOver (bool b_state);
  [[nodiscard]] bool IsGameOver () const;
  [[nodiscard]] Entity *CreateEntity ();
  [[nodiscard]] bool IsDownLMB () const;
  void DeleteEntity (Entity *ent);
  std::vector<Entity *> &GetEntities ();
  void RegisterDownLMB ();
  //Score interface
  void SetScore (Sint32 s);
  void IncrementScore ();
  void PostScore ();
  [[nodiscard]] void GetHighscore ();
  void DeleteScores ();
};

#endif /* APP_MODULE_H */
