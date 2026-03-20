/* Copyright (c) 2026. LetTheMiceFree. */
#include "sprite.h"

#include "app.h"

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

SpriteComponent::SpriteComponent (App *app,
                                  const std::string &path)
    //: resource (new SpriteResource (renderer, path.c_str ()))
{
  resource = app->sprite_resource_factory->MakeSprite (app->GetRenderer (), path);
}

void
SpriteComponent::UpdateComponent (App *app, Entity *owner)
{
}

SpriteResource *
SpriteComponent::GetSprite () const
{
  return resource;
}
