/* Copyright (c) 2026. LetTheMiceFree. */
#include "sprite.h"

#include <SDL3_image/SDL_image.h>

SpriteResource::SpriteResource (SDL_Renderer *renderer, const char *path)
    : texture (IMG_LoadTexture (renderer, path))
{
  if (!texture)
    {
      SDL_LogError (1, "Missing texture!");
    }
}

SDL_Texture *
SpriteResource::GetTexture () const
{
  return texture;
}

SpriteComponent::SpriteComponent (SDL_Renderer *renderer,
                                  const std::string &path)
    : sprite (new SpriteResource (renderer, path.c_str ()))
{
}

void
SpriteComponent::UpdateComponent (App *app, Entity *owner)
{
}

SpriteResource *
SpriteComponent::GetSprite () const
{
  return sprite;
}
