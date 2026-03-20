/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef SPRITE_COMPONENT_MODULE_H
#define SPRITE_COMPONENT_MODULE_H

#include "component.h"

#include <SDL3/SDL_render.h>
#include <string>

struct SpriteResource
{
private:
  SDL_Texture *texture;

public:
  SpriteResource (SDL_Renderer *renderer, const char *path);
  ~SpriteResource () = default;

  [[nodiscard]] SDL_Texture *GetTexture () const;
};

/**
 * @brief Usine de SpriteResources, maintient la "map" de tous les
 * poids-mouches représentant les images.
 */
class SpriteResourceFactory
{
  /**
   * @brief La "map", j'utilise une approche 'custom' avec SDL_Properties
   * plutôt que std::map<T> ou une autre technique.
   */
  SDL_PropertiesID sprite_resources;

public:
  SpriteResourceFactory () = default;
  ~SpriteResourceFactory () = default;

  /**
   * @brief Fonction pour obtenir une resource en fonction de son nom.
   * @param key
   * @return
   */
  SpriteResource *
  MakeSprite (SDL_Renderer *renderer, const std::string &key) const
  {
    void *raw_resource
        = SDL_GetPointerProperty (sprite_resources, key.c_str (), nullptr);
    if (raw_resource)
      {
        return static_cast<SpriteResource *> (raw_resource);
      }
    auto new_resource = new SpriteResource (renderer, key.c_str ());
    SDL_SetPointerProperty (sprite_resources, key.c_str (), new_resource);
    return new_resource;
  }
};

class SpriteComponent : public Component
{
  SpriteResource *sprite;

public:
  SpriteComponent (SDL_Renderer *renderer, const std::string &image);
  ~SpriteComponent () override = default;

  void UpdateComponent (App *app, Entity *owner) override;

  [[nodiscard]] SpriteResource *GetSprite () const;
};

#endif /* SPRITE_COMPONENT_MODULE_H */
