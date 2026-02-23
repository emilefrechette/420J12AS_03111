
#include "component.h"

#include <cstdlib>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

#include <SDL3/SDL.h>

#include "game.h"

/*
 * Question 5) Créez les composants nécessaires afin de supporter la mécanique de collecte des
 * jetons sur le sac à dos. Lorsqu'un jeton est déposé sur le sac à dos, il faut l'enlever de
 * l'affichage ou le détruire, et ajouter le pointage correspondant dans le sac à dos.
 * (Affichez le nombre de points à l'écran avec SDL_RenderDebugTextFormat.) 15%
 *
 *
 */

Sint32
main (int argc, char *argv[])
{
	GameApp *app = new GameApp ();
	bool running = true;
	uint64_t last_time = SDL_GetPerformanceCounter ();

	while (running == true)
		{
			SDL_Event event;
			while (SDL_PollEvent (&event) == true)
				{
					if (event.type == SDL_EVENT_QUIT)
						{
							running = false;
							break;
						}
					if (event.type == SDL_EVENT_MOUSE_MOTION)
						{
							for (const auto &ent : app->entities)
								{
									if (auto *transform
										= ent->GetComponentIfExists<TransformComponent> ();
										const auto *mouse_interact
										= ent->GetComponentIfExists<MouseInteractComponent> ())
										{
											if (mouse_interact->mouse_state == CLICKED)
												{
													transform->position.x += event.motion.xrel;
													transform->position.y += event.motion.yrel;
												}
										}
								}
						}
					if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
						{
							for (const auto &ent : app->entities)
								{
									if (auto *mouse_interact
										= ent->GetComponentIfExists<MouseInteractComponent> ())
										{
											if (mouse_interact->mouse_state == SELECTED)
												{
													mouse_interact->mouse_state = CLICKED;
													break;
												}
										}
								}
							break;
						}
					if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
						{
							for (const auto &ent : app->entities)
								{
									if (auto *mouse_interact
										= ent->GetComponentIfExists<MouseInteractComponent> ())
										{
											mouse_interact->mouse_state = UNSELECTED;
										}
								}
							const auto *bag_transform
								= app->bag->GetComponentIfExists<TransformComponent> ();
							auto *bag_point_acc
								= app->bag->GetComponentIfExists<PointAccumulatorComponent> ();
							for (size_t i = 0; i < app->entities.size (); ++i)
								{
									if (const auto *tok
										= dynamic_cast<TokenEntity *> (app->entities[i]))
										{
											const auto *tok_transform
												= tok->GetComponentIfExists<TransformComponent> ();
											const auto *tok_point
												= tok->GetComponentIfExists<PointComponent> ();

											if ((tok_transform->position.x
													 > bag_transform->position.x
												 && tok_transform->position.x
														< bag_transform->position.x
															  + bag_transform->size.x)
												&& (tok_transform->position.y
														> bag_transform->position.y
													&& tok_transform->position.y
														   < bag_transform->position.y
																 + bag_transform->size.y))
												{
													bag_point_acc->total += tok_point->score;
													delete tok;
													app->entities.erase (app->entities.begin ()
																		 + static_cast<Sint32>(i));
													--i;
												}
										}
								}
							break;
						}
				}

			SDL_GetMouseState (&app->mouse_pos.x, &app->mouse_pos.y);

			const uint64_t freq = SDL_GetPerformanceFrequency ();
			const uint64_t current_time = SDL_GetPerformanceCounter ();
			const float delta_time
				= static_cast<float> (current_time - last_time) / static_cast<float> (freq);
			last_time = current_time;
			app->CalculateFPS (delta_time);

			for (const auto &ent : app->entities)
				{
					ent->Update (app);
				}

			SDL_SetRenderDrawColor (app->renderer, 12, 12, 44, 255);
			SDL_RenderClear (app->renderer);

			SDL_Point win_size = { 0 };
			SDL_GetWindowSize (app->window, &win_size.x, &win_size.y);

			SDL_SetRenderDrawColor (app->renderer, 42, 42, 104, 255);
			SDL_RenderLine (app->renderer, 0.f, 0.f, static_cast<float> (win_size.x),
							static_cast<float> (win_size.y));
			SDL_RenderLine (app->renderer, 0.f, static_cast<float> (win_size.y),
							static_cast<float> (win_size.x), 0.f);

			static float displayed;
			static float count;
			if (count <= 0.f)
				{
					displayed = app->avg_fps;
					count = 90.f;
				}
			else
				{
					count -= delta_time * 1000.f;
				}
			SDL_SetRenderDrawColor (app->renderer, 255, 255, 255, 255);
			SDL_RenderDebugTextFormat (app->renderer, static_cast<float> (win_size.x) - 115, 5,
									   "%.2f FPS", displayed);
			{
				const auto *bag_point_acc
					= app->bag->GetComponentIfExists<PointAccumulatorComponent> ();
				SDL_RenderDebugTextFormat (app->renderer, static_cast<float> (win_size.x) - 115, 25,
										   "Score: %d", bag_point_acc->total);
			}
			for (const auto &ent : app->entities)
				{
					if (const auto draw = dynamic_cast<Drawable *> (ent))
						{
							draw->Draw (app);
						}
				}
			SDL_RenderPresent (app->renderer);
		}
	return 0;
}
