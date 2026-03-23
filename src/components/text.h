/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef TEXT_MODULE_H
#define TEXT_MODULE_H

#include <string>

#include "component.h"

#include <SDL3_ttf/SDL_ttf.h>

class TextComponent : public Component
{
  TTF_Text *glyph = nullptr;

public:
  TextComponent () = default;
  TextComponent (App *app, const std::string &_content);
  ~TextComponent () override = default;

  [[nodiscard]] TTF_Text *GetGlyph () const;
  void UpdateComponent (App *app, Entity *owner) override;
  void UpdateGlyph (App *app, const std::string &_content);
};

#endif /* TEXT_MODULE_H */
