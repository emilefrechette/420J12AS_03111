/* Copyright (c) 2026. LetTheMiceFree. */
#include <SDL3/SDL.h>
#include <vector>

class TransformComponent;
typedef enum
{
  KEYSTATE_INACTIVE,
  KEYSTATE_RELEASED,
  KEYSTATE_ACTIVE,
  KEYSTATE_PRESSED,
} KeystateType;

class BulletMemoryPool;
class Entity;

class App final
{
public:
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  bool b_game_over = false;
  KeystateType keys[SDL_SCANCODE_COUNT]{};
  std::vector<SDL_Scancode> updated_keys;
  std::vector<Entity *> entities = std::vector<Entity *> ();
  Entity *player = nullptr;
  BulletMemoryPool *bullets = nullptr;
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
};

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
    ColorComponent *color = owner->GetComponent<ColorComponent> ();
    TransformComponent *transform = owner->GetComponent<TransformComponent> ();
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
        if (b_fill)
          {
            SDL_RenderFillRect (app->renderer, &dst);
          }
        else
          {
            SDL_RenderRect (app->renderer, &dst);
          }
      }
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

class Guy : public Entity
{
public:
  Guy ()
  {
    AddComponent (new BoxComponent (true));
    AddComponent (new ColorComponent ({ 0u, 255u, 255u, 255u }));
    AddComponent (new TransformComponent ({ 50.f, 600.f }, { 64.f, 64.f }));
  }
};

class Bullet : public Entity
{
public:
  bool b_in_use = false;
  Bullet ()
  {
    AddComponent (new BoxComponent (true));
    AddComponent (new ColorComponent ({ 255u, 0u, 0u, 255u }));
    AddComponent (new TransformComponent ({ -100.f, 100.f }, { 20.f, 20.f }));
    AddComponent (new VelocityComponent ({ 0.f, 0.f }));
  };
  Bullet (SDL_FPoint _pos)
  {
    AddComponent (new BoxComponent (true));
    AddComponent (new ColorComponent ({ 255u, 0u, 0u, 255u }));
    AddComponent (new TransformComponent (_pos, { 20.f, 20.f }));
    AddComponent (new VelocityComponent ({ .2f, 0.f }));
  }

  virtual void
  Update (App *app) override
  {
    Entity::Update (app);

    TransformComponent *transform = GetComponent<TransformComponent> ();
    if (transform)
      {
        if (transform->position.x > 1280)
          {
            b_in_use = false;
          }
      }
  }
};

class BulletMemoryPool final
{
public:
  BulletMemoryPool () = default;
  ~BulletMemoryPool () = default;

  Bullet *
  Acquire (SDL_FPoint pos, SDL_FPoint vel)
  {
    for (auto &bullet : bullets)
      {
        Bullet *b = &bullet;
        if (b->b_in_use == true)
          {
            continue;
          }
        b->GetComponent<TransformComponent> ()->position = pos;
        b->GetComponent<VelocityComponent> ()->delta = vel;
        b->b_in_use = true;
        return b;
      }
    return nullptr;
  }

  static const Sint32 MAX_BULLETS = 25;
  Bullet bullets[MAX_BULLETS]{};
};

Sint32
main (Sint32 argc, char *argv[])
{

  App *app = new App ();
  SDL_Init (SDL_INIT_VIDEO);
  app->window = SDL_CreateWindow ("Object pool", 1280, 1280, 0);
  app->renderer = SDL_CreateRenderer (app->window, 0);
  SDL_SetRenderVSync (app->renderer, true);
  app->bullets = new BulletMemoryPool ();

  for (auto &bullet : app->bullets->bullets)
    {
      app->RegisterEntity (&bullet);
    }

  app->player = new Guy ();
  app->RegisterEntity (app->player);

  app->now = SDL_GetPerformanceCounter ();

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

      static float shoot_cd;
      if (shoot_cd > 0.f)
        {
          shoot_cd -= 100 * app->dt_ms;
        }
      if (app->keys[SDL_SCANCODE_SPACE] == KEYSTATE_ACTIVE && shoot_cd <= 0.f)
        {

          app->bullets->Acquire (
              app->player->GetComponent<TransformComponent> ()->position,
              { 400.f, 0.f });
          shoot_cd = App::SHOOT_DELAY;
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
