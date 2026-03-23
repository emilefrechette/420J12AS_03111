/* Copyright (c) 2026. LetTheMiceFree. */
#include "render.h"

#include "app.h"
#include "box.h"
#include "color.h"
#include "entity.h"
#include "text.h"
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
  const TextComponent *text = owner->GetComponent<TextComponent> ();
  const ColorComponent *color = owner->GetComponent<ColorComponent> ();

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
      if (color)
        {
          const auto [r, g, b, a] = color->GetValue ();
          SDL_SetRenderDrawColor (renderer, r, g, b, a);
        }
      else
        {
          SDL_SetRenderDrawColor (renderer, 255u, 255u, 255u, 255u);
        }
      SDL_RenderRect (renderer, &dst);
    }
  if (text)
    {
      if (color)
        {
          const auto [r, g, b, a] = color->GetValue ();
          TTF_SetTextColor (text->GetGlyph (), r, g, b, a);
        }
      else
        {
          TTF_SetTextColor (text->GetGlyph (), 255u, 255u, 255u, 255u);
        }
      TTF_DrawRendererText (text->GetGlyph (), dst.x, dst.y);
    }
}
