/* Copyright (c) 2026. LetTheMiceFree. */
#include "click.h"

#include "app.h"
#include "entity.h"
#include "hover.h"

ClickComponent::ClickComponent (const ClickEffect _effect,
                                const SDL_Color _active_color)
    : active_color (_active_color), effect (_effect)
{
}

bool
ClickComponent::GetState () const
{
  return b_state;
}

SDL_Color
ClickComponent::GetActiveColor () const
{
  return active_color;
}

void
ClickComponent::UpdateComponent (App *app, Entity *owner)
{
  if (!app->IsDownLMB ())
    {
      b_state = false;
      return;
    }
  const auto *hover = owner->GetComponent<HoverComponent> ();
  if (!hover)
    {
      b_state = false;
      return;
    }
  if (hover->GetState ())
    {

      switch (effect)
        {
        case CLICK_EFFECT_INC_SCORE:
          {
            app->IncrementScore ();
            break;
          }
        case CLICK_EFFECT_SEND_SCORE:
          {
            app->PostScore();
            break;
          }
        case CLICK_EFFECT_GET_HIGHSCORE:
          {
           app->GetHighscore();
            break;
          }
        case CLICK_EFFECT_DELETE_ALL_SCORE:
          {
            app->DeleteScores();
            break;
          }
        default:
          {
            break;
          }
        }
      b_state = true;
    }
}
