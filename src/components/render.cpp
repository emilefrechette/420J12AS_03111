/* Copyright (c) 2026. LetTheMiceFree. */
#include "render.h"

#include "app.h"
#include "box.h"
#include "entity.h"
#include "sprite.h"
#include "transform.h"

class BoxComponent;

RenderComponent::RenderComponent (SDL_Renderer *_renderer)
    : renderer (_renderer)
{
}

void
RenderComponent::UpdateComponent (App *app, Entity *owner)
{
  const TransformComponent *transform
      = owner->GetComponent<TransformComponent> ();
  const BoxComponent *box = owner->GetComponent<BoxComponent> ();
  const SpriteComponent *sprite = owner->GetComponent<SpriteComponent> ();

  if (!transform)
    {
      return;
    }

  const SDL_FRect dst = {
    transform->GetPosition ().x,
    transform->GetPosition ().y,
    transform->GetSize ().x,
    transform->GetSize ().y,
  };

  if (box)
    {
      SDL_SetRenderDrawColor (renderer, 255u, 255u, 255u, 255u);
      SDL_RenderRect (renderer, &dst);
    }

  if (sprite)
    {
      SpriteResource *resource = sprite->GetSprite ();
      SDL_RenderTexture (renderer, resource->GetTexture (), nullptr, &dst);
    }
}
