/* Copyright (c) 2026. LetTheMiceFree. */
#include "game.h"

#include "component.h"

#include <numeric>

GameApp::GameApp ()
{
	if (SDL_Init (SDL_INIT_VIDEO) == false)
		{
			SDL_LogCritical (1, "SDL failed to initialize! %s", SDL_GetError ());
			abort ();
		}
	window = SDL_CreateWindow (AppTitle.c_str (), 800, 600, 0);
	if (window == nullptr)
		{
			SDL_LogCritical (1, "SDL failed to create window! %s", SDL_GetError ());
			abort ();
		}
	renderer = SDL_CreateRenderer (window, nullptr);
	if (renderer == nullptr)
		{
			SDL_LogCritical (1, "SDL failed to create renderer! %s", SDL_GetError ());
			abort ();
		}

	SDL_SetRenderVSync (renderer, true);

	SDL_Time time;
	SDL_GetCurrentTime (&time);
	SDL_srand (time);

	bag = new BagEntity (this);
	entities.push_back (bag);

	const Sint32 token_random_count = 9 + SDL_rand (11);
	for (Sint32 i = 0; i < token_random_count; i++)
		{
			entities.push_back (new TokenEntity (this));
		}
}

GameApp::~GameApp ()
{
	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (window);
	SDL_Quit ();
}

void
GameApp::CalculateFPS (const float DeltaTime)

{
	frames.push_back (DeltaTime);
	if (frames.size () > MAX_SAMPLES)
		{
			frames.erase (frames.begin ());
		}
	const float sum = std::accumulate (frames.begin (), frames.end (), 0.0f);
	const float average = sum / static_cast<float> (frames.size ());
	avg_fps = average > 0 ? 1.0f / average : 0;
}

void
Drawable::Draw (GameApp *app)
{
	const auto *filter_entity = dynamic_cast<Entity *> (this);
	SDL_FRect dst = { 0 };
	const TransformComponent *transform
		= filter_entity->GetComponentIfExists<TransformComponent> ();
	dst.x = transform->position.x;
	dst.y = transform->position.y;
	dst.w = transform->size.x;
	dst.h = transform->size.y;

	auto [r, g, b, a] = filter_entity->GetComponentIfExists<ColorComponent> ()->color;
	SDL_SetRenderDrawColor (app->renderer, r, g, b, a);

	SDL_RenderFillRect (app->renderer, &dst);
}

TokenEntity::TokenEntity (const GameApp *app)
{
	SDL_Point window_size = { 0 };
	SDL_GetWindowSize (app->window, &window_size.x, &window_size.y);

	const auto pos = (SDL_FPoint){ static_cast<float> (SDL_rand (window_size.x)),
								   static_cast<float> (SDL_rand (window_size.y / 2)) };
	constexpr auto size = (SDL_FPoint){ 40.f, 40.f };
	auto color = (SDL_Color){ 255u, 255u, 0u, 255u };

	components.push_back (new TransformComponent (pos, size));
	components.push_back (new MouseInteractComponent ());
	switch (SDL_rand (20))
		{
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
			components.push_back (new PointComponent (SUPER));
			color = (SDL_Color){ 155u, 255u, 65u, 255u };
			break;
		case 17:
		case 18:
		case 19:
			components.push_back (new PointComponent (OMEGA));
			color = (SDL_Color){ 0u, 123u, 255u, 255u };
			break;
		default:
			components.push_back (new PointComponent ());
			break;
		}
	components.push_back (new ColorComponent (color));
}

BagEntity::BagEntity (const GameApp *app)
{
	SDL_Point window_size = { 0 };
	SDL_GetWindowSize (app->window, &window_size.x, &window_size.y);

	const auto pos = (SDL_FPoint){ static_cast<float> (window_size.x) / 2.f - 60.f,
								   static_cast<float> (window_size.y) * .75f - 75.f };
	constexpr auto size = (SDL_FPoint){ 120.f, 150.f };
	constexpr auto color = (SDL_Color){ 255u, 125u, 0u, 255u };

	components.push_back (new TransformComponent (pos, size));
	components.push_back (new ColorComponent (color));
	components.push_back (new PointAccumulatorComponent ());
}
