/*
 * Démo - Importation et utilisation de textures.
 */

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

SDL_Window *win = nullptr;
SDL_Renderer *rend = nullptr;
SDL_Texture *sprites = nullptr;

int main(int argc, char *argv[])
{
	/* Initialisation. */
	SDL_Init(SDL_INIT_VIDEO);
	win = SDL_CreateWindow("Gestion des images", 480, 480, 0);
	rend = SDL_CreateRenderer(win, nullptr);

	/* Chargement de l'image. */
	sprites = IMG_LoadTexture(rend, "sheet.png");
	if (sprites == nullptr)
		{
			SDL_LogWarn(0, "Failed to load image! %s", SDL_GetError());
		}
	SDL_SetTextureScaleMode(sprites, SDL_SCALEMODE_NEAREST);

	/* Game loop pattern. */
	while (1)
		{
			/* Event queue. */
			SDL_Event e;
			while (SDL_PollEvent(&e) == true)
				{
					if (e.type == SDL_EVENT_QUIT)
						{
							return 1;
						}
				}
			SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
			SDL_RenderClear(rend);

			const int tile_size = 16;
			SDL_FRect src = {
				4 * tile_size,
				5 * tile_size,
				tile_size,
				tile_size,
		};
			for (int i = 0; i < 10; i++)
				{

					for (int j = 0; j < 10; j++)
						{
							float tile_scale = 5.f;
							SDL_FRect dst = {
								(float) i * tile_size * tile_scale,
								(float) j * tile_size * tile_scale,
								tile_size * tile_scale,
								tile_size * tile_scale,
						};
							SDL_RenderTexture(rend, sprites, /* src */ &src, /* dst */ &dst);
						}
				}

			SDL_RenderPresent(rend);
		}

	SDL_Log("hello, SDL");

	return 0;
}
