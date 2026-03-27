/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef COMPONENT_MODULE_H
#define COMPONENT_MODULE_H

class App;
class Entity;

class Component
{
public:
  Component () = default;
  virtual ~Component () = default;

  virtual void UpdateComponent (App *app, Entity *owner) = 0;
};

#endif /* COMPONENT_MODULE_H */
