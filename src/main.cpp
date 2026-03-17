/* Copyright (c) 2026. LetTheMiceFree. */

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <algorithm>
#include <vector>

class App;
class TransformComponent;
typedef enum
{
  KEYSTATE_INACTIVE,
  KEYSTATE_RELEASED,
  KEYSTATE_ACTIVE,
  KEYSTATE_PRESSED,
} KeystateType;

class Entity;

class Component
{
public:
  Component () = default;
  virtual ~Component () = default;

  virtual void UpdateComponent (App *app, Entity *owner) = 0;
};

class Entity
{
public:
  Uint64 flag = 0u;
  std::vector<Component *> components{};

  Entity () = default;
  virtual ~Entity () = default;

  void
  AddComponent (Component *comp)
  {
    components.push_back (comp);
  }

  template <typename T>
  bool
  HasComponent ()
  {
    for (const auto &comp : components)
      {
        if (dynamic_cast<T> (comp))
          {
            return true;
          }
      }
    return false;
  }

  template <typename T>
  T *
  GetComponent ()
  {
    for (const auto &comp : components)
      {
        if (T *my_type = dynamic_cast<T *> (comp))
          {
            return my_type;
          }
      }
    return nullptr;
  }

  virtual void
  Update (App *app)
  {
    for (const auto &comp : components)
      {
        comp->UpdateComponent (app, this);
      }
  }
};

class App final
{
public:
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  bool b_game_over = false;
  KeystateType keys[SDL_SCANCODE_COUNT]{};
  std::vector<SDL_Scancode> updated_keys;
  std::vector<Entity *> entities = std::vector<Entity *> ();
  SDL_Texture *texture_array[4]{ nullptr };
  Entity *asteroid = nullptr;
  Uint64 now = 0u;
  Uint64 last = 0u;
  float fps = 0.f;
  float dt_ms = 0.f;
  static constexpr float SHOOT_DELAY = 20.f;
  App () = default;
  ~App () = default;

  void
  RegisterEntity (Entity *ent)
  {
    entities.push_back (ent);
  }
  void
  DeleteEntity (Entity *ent)
  {
    if (ent == nullptr)
      return;

    auto it = std::find (entities.begin (), entities.end (), ent);

    if (it != entities.end ())
      {
        entities.erase (it);
        delete ent;
      }
  }
};

class ColorComponent : public Component
{
public:
  SDL_Color value{};
  ColorComponent (SDL_Color _value) : value (_value) {};

  void
  UpdateComponent (App *app, Entity *owner) override
  {
  }
};

class SpriteComponent : public Component
{
public:
  Sint32 sprite_index;
  bool b_has_src = false;
  SDL_FRect src{ 0 };

  SpriteComponent () = default;
  SpriteComponent (const Sint32 _i) : sprite_index (_i) {}
  SpriteComponent (const Sint32 _i, const SDL_FRect _src)
      : sprite_index (_i), src (_src)
  {
    b_has_src = true;
  }

  void
  UpdateComponent (App *app, Entity *owner) override
  {
  }
};

class TransformComponent : public Component
{
public:
  SDL_FPoint position{};
  SDL_FPoint size{};

  TransformComponent (SDL_FPoint _size) : size (_size) {}
  TransformComponent (SDL_FPoint _pos, SDL_FPoint _size)
      : position (_pos), size (_size)
  {
  }

  void
  UpdateComponent (App *app, Entity *owner) override
  {
  }
};

class BoxComponent : public Component
{
public:
  bool b_fill;
  BoxComponent (bool _fill) : b_fill (_fill) {}

  void
  UpdateComponent (App *app, Entity *owner) override
  {
  }
};

class RotateComponent : public Component
{
public:
  float angle = 0.f;
  float rate = 1.f;
  RotateComponent () = default;
  RotateComponent (const float _rate) : rate (_rate) {}

  void
  UpdateComponent (App *app, Entity *owner) override
  {
    angle += rate * app->dt_ms;
  }
};

class VelocityComponent : public Component
{
public:
  SDL_FPoint delta{};
  VelocityComponent (SDL_FPoint _delta) : delta (_delta) {};

  void
  UpdateComponent (App *app, Entity *owner) override
  {
    TransformComponent *transform = owner->GetComponent<TransformComponent> ();
    if (transform != nullptr)
      {
        transform->position.x += delta.x * app->dt_ms;
        transform->position.y += delta.y * app->dt_ms;
      }
  }
};

class VisibilityComponent : public Component
{
public:
  bool b_is_visible;

  VisibilityComponent () = default;
  VisibilityComponent (bool _vis) : b_is_visible (_vis) {}

  void
  UpdateComponent (App *app, Entity *owner) override
  {
  }
};

class RenderComponent : public Component
{
public:
  RenderComponent () = default;

  void
  UpdateComponent (App *app, Entity *owner) override
  {
    BoxComponent *box = owner->GetComponent<BoxComponent> ();
    ColorComponent *color = owner->GetComponent<ColorComponent> ();
    SpriteComponent *sprite = owner->GetComponent<SpriteComponent> ();
    RotateComponent *rotate = owner->GetComponent<RotateComponent> ();
    TransformComponent *transform = owner->GetComponent<TransformComponent> ();
    VisibilityComponent *visibility
        = owner->GetComponent<VisibilityComponent> ();
    if (color && transform)
      {
        SDL_FRect dst = {
          transform->position.x,
          transform->position.y,
          transform->size.x,
          transform->size.y,
        };
        SDL_SetRenderDrawColor (app->renderer, color->value.r, color->value.g,
                                color->value.b, color->value.a);
        if (box->b_fill)
          {
            SDL_RenderFillRect (app->renderer, &dst);
          }
        else
          {
            SDL_RenderRect (app->renderer, &dst);
          }

        if (sprite)
          {
            SDL_RenderTextureRotated (
                app->renderer, app->texture_array[sprite->sprite_index],
                sprite->b_has_src ? &sprite->src : nullptr, &dst,
                rotate ? rotate->angle : 0.f, nullptr, SDL_FLIP_NONE);
          }
      }
  }
};

class Asteroid : public Entity
{
public:
  Asteroid (SDL_FPoint _pos)
  {
    SDL_FPoint dims = { 160.f, 160.f };
    AddComponent (new BoxComponent (false));
    AddComponent (new ColorComponent ({ 0u, 255u, 255u, 255u }));
    AddComponent (new TransformComponent (
        { _pos.x - dims.x / 2, _pos.y - dims.y / 2 }, dims));
    AddComponent (new RotateComponent (15.f));
    AddComponent (new SpriteComponent (0));
    AddComponent (new RenderComponent ());
  }
};

class AsteroidPart : public Entity
{
public:
  AsteroidPart (const SDL_FPoint _pos, const SDL_FRect _section, const SDL_FPoint _vel, const float _init_angle)
  {
    SDL_FPoint dims = { 80.f, 80.f };
    AddComponent (new BoxComponent (false));
    AddComponent (new ColorComponent ({ 0u, 255u, 255u, 255u }));
    AddComponent (new VelocityComponent (_vel));
    AddComponent (new TransformComponent (
        { _pos.x - dims.x / 2, _pos.y - dims.y / 2 }, dims));
    RotateComponent *rotate = new RotateComponent (15.f);
    rotate->angle = _init_angle;
    AddComponent (rotate);
    AddComponent (new SpriteComponent (0, _section));
    AddComponent (new RenderComponent ());
  }
};

Sint32
main (Sint32 argc, char *argv[])
{
  // Init phase
  App *app = new App ();
  SDL_Init (SDL_INIT_VIDEO);
  app->window = SDL_CreateWindow ("Object pool", 1280, 1280, 0);
  app->renderer = SDL_CreateRenderer (app->window, 0);
  SDL_SetRenderVSync (app->renderer, true);

  app->asteroid = new Asteroid ({ 640.f, 640.f });
  app->RegisterEntity (app->asteroid);

  app->texture_array[0] = IMG_LoadTexture (app->renderer, "asteroid.png");
  if (app->texture_array[0] == nullptr)
    {
      SDL_LogWarn (1, "Failed to load image! %s", SDL_GetError ());
    }

  app->now = SDL_GetPerformanceCounter ();

  // Update phase
  while (app->b_game_over == false)
    {
      SDL_Event e;
      app->last = app->now;
      app->now = SDL_GetPerformanceCounter ();
      Uint64 span = app->now - app->last;
      app->dt_ms = (float)span / (float)SDL_GetPerformanceFrequency ();

      while (SDL_PollEvent (&e) == true)
        {
          switch (e.type)
            {
            case SDL_EVENT_QUIT:
              {
                app->b_game_over = true;
                break;
              }
            case SDL_EVENT_KEY_DOWN:
              {
                if (app->keys[e.key.scancode] != KEYSTATE_ACTIVE)
                  {
                    app->keys[e.key.scancode] = KEYSTATE_PRESSED;
                    app->updated_keys.push_back (e.key.scancode);
                  }
                break;
              }
            case SDL_EVENT_KEY_UP:
              {
                app->keys[e.key.scancode] = KEYSTATE_RELEASED;
                app->updated_keys.push_back (e.key.scancode);
                break;
              }
            default:
              {
                break;
              }
            }
        }

      if (app->keys[SDL_SCANCODE_SPACE] == KEYSTATE_PRESSED)
        {
          const float old_angle = app->asteroid->GetComponent<RotateComponent> ()->angle;
          app->DeleteEntity (app->asteroid);

          app->RegisterEntity (new AsteroidPart (
              { 600.f, 600.f }, { 0.f, 0.f, 80.f, 80.f }, { -100.f, -100.f }, old_angle));
          app->RegisterEntity (new AsteroidPart (
              { 680.f, 600.f }, { 80.f, 0.f, 80.f, 80.f }, { 100.f, -100.f }, old_angle));
          app->RegisterEntity (new AsteroidPart (
              { 600.f, 680.f }, { 0.f, 80.f, 80.f, 80.f }, { -100.f, 100.f }, old_angle));
          app->RegisterEntity (new AsteroidPart (
              { 680.f, 680.f }, { 80.f, 80.f, 80.f, 80.f }, { 100.f, 100.f }, old_angle));
        }

      SDL_SetRenderDrawColor (app->renderer, 0, 55, 0, 255);
      SDL_RenderClear (app->renderer);

      for (const auto &ent : app->entities)
        {
          ent->Update (app);
        }

      SDL_RenderPresent (app->renderer);
      for (const SDL_Scancode &key : app->updated_keys)
        {
          KeystateType *keystate_ref = &app->keys[key];
          switch (*keystate_ref)
            {
            case KEYSTATE_PRESSED:
              {
                *keystate_ref = KEYSTATE_ACTIVE;
                break;
              }
            case KEYSTATE_RELEASED:
              {
                *keystate_ref = KEYSTATE_INACTIVE;
                break;
              }
            default:
              {
                break;
              }
            }
        }
      app->updated_keys.clear ();
    }
  return 0;
}
