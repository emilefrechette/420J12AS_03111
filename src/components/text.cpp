/* Copyright (c) 2026. LetTheMiceFree. */
#include "text.h"

#include "app.h"

TextComponent::TextComponent (App *app, const std::string &_content)
{
  glyph = TTF_CreateText(app->GetTextEngine (), app->GetFont(), _content.c_str(), _content.length ());
}

TTF_Text *
TextComponent::GetGlyph () const
{
  return glyph;
}

void
TextComponent::UpdateComponent (App *app, Entity *owner)
{
}
