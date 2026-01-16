#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

static constexpr Sint32 TILE_SIZE = 32;
static constexpr Sint32 ANIM_ROW_BEGIN = 0;
static constexpr Sint32 ANIM_ROW_END = 0;
static constexpr Sint32 ANIM_COL_BEGIN = 0;
static constexpr Sint32 ANIM_COL_END = 6;
static constexpr Sint32 PRESENT_SIZE = 8;

static Uint32 TimerCallback(void *userdata, SDL_TimerID timerID, Uint32 interval)
{
	bool *updateFlag = static_cast<bool *>(userdata);
	*updateFlag = true;
	return interval;
}

class GameApp {

		SDL_Window *window = nullptr;
		SDL_Renderer *renderer = nullptr;
		SDL_Texture *spritesheet = nullptr;

		float animTimer = 0.0f;
		int currentCol = ANIM_COL_BEGIN;
		int currentRow = ANIM_ROW_BEGIN;
		const float frameDuration = 0.15f;

		float colorTime = 0.0f;
		Uint8 r = 0, g = 0, b = 0;

		TTF_Font *font = nullptr;
		TTF_TextEngine *textEngine = nullptr;
		TTF_Text *fpsText = nullptr;

		std::vector<float> frameTimes;
		const size_t MAX_SAMPLES = 100;
		bool shouldUpdateText = false;
		SDL_TimerID fpsTimerID;
		float currentFPS = 0.0f;

	public:
		GameApp()
		{
			if (SDL_Init(SDL_INIT_VIDEO) == false)
				{
					SDL_LogCritical(1, "SDL failed to initialize! %s", SDL_GetError());
					abort();
				}
			window = SDL_CreateWindow("Hello!", 800, 600, 0);
			if (window == nullptr)
				{
					SDL_LogCritical(1, "SDL failed to create window! %s", SDL_GetError());
					abort();
				}
			renderer = SDL_CreateRenderer(window, nullptr);
			if (renderer == nullptr)
				{
					SDL_LogCritical(1, "SDL failed to create renderer! %s", SDL_GetError());
					abort();
				}
			SDL_SetDefaultTextureScaleMode(renderer, SDL_SCALEMODE_NEAREST);
			spritesheet = IMG_LoadTexture(renderer, "assets/spritesheet.png");
			if (spritesheet == nullptr)
				{
					SDL_LogWarn(0, "SDL_image failed to load texture '%s'! %s", "assets/spritesheet.png",
								SDL_GetError());
				}

			if (TTF_Init() == false)
				{
					SDL_LogCritical(1, "SDL_ttf failed to initialize! %s", SDL_GetError());
					abort();
				}
			textEngine = TTF_CreateRendererTextEngine(renderer);
			if (textEngine == nullptr)
				{
					SDL_LogCritical(1, "SDL_ttf failed to create text engine!! %s", SDL_GetError());
					abort();
				}
			font = TTF_OpenFont("assets/font.ttf", 24);
			if (font == nullptr)
				{
					SDL_LogWarn(0, "SDL_ttf failed to load font '%s'! %s", "assets/font.ttf", SDL_GetError());
				}
			fpsText = TTF_CreateText(textEngine, font, "FPS: 0", 20);
			if (fpsText == nullptr)
				{
					SDL_LogWarn(0, "SDL_ttf failed to create text '%s'! %s", "FPS: 0", SDL_GetError());
				}
			if (TTF_SetTextColor(fpsText, 255, 255, 255, 255) == false)
				{
					SDL_LogWarn(0, "SDL_ttf failed to set text color to (255, 255, 255, 255)! %s", SDL_GetError());
				}

			fpsTimerID = SDL_AddTimer(2000, TimerCallback, &shouldUpdateText);
		}

		~GameApp()
		{
			SDL_RemoveTimer(fpsTimerID);
			TTF_DestroyText(fpsText);
			TTF_DestroyRendererTextEngine(textEngine);
			TTF_CloseFont(font);
			SDL_DestroyTexture(spritesheet);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_Quit();
			SDL_Quit();
		}

		void CalculateFPS(const float deltaTime)
		{
			frameTimes.push_back(deltaTime);
			if (frameTimes.size() > MAX_SAMPLES)
				{
					frameTimes.erase(frameTimes.begin());
				}
			const float sum = std::accumulate(frameTimes.begin(), frameTimes.end(), 0.0f);
			const float avgDelta = sum / static_cast<float>(frameTimes.size());
			currentFPS = (avgDelta > 0) ? 1.0f / avgDelta : 0;

			if (shouldUpdateText)
				{
					std::string fpsStr = "FPS: " + std::to_string(static_cast<int>(currentFPS));
					TTF_SetTextString(fpsText, fpsStr.c_str(), 0);
					shouldUpdateText = false; // Reset the flag
				}
		}

		void AdvanceAnimation(const float deltaTime)
		{
			animTimer += deltaTime;
			if (animTimer >= frameDuration)
				{
					animTimer = 0.0f;
					currentCol++;

					if (currentCol > ANIM_COL_END)
						{
							currentCol = ANIM_COL_BEGIN;
							currentRow++;
						}

					if (currentRow > ANIM_ROW_END)
						{
							currentRow = ANIM_ROW_BEGIN;
						}
				}
		}

		void UpdateBackgroundTint(const float deltaTime)
		{
			constexpr float speed = 1.2f;
			colorTime += deltaTime * speed;

			constexpr float Amplitude = 60.0f;
			constexpr float MidPoint = 60.0f;

			r = static_cast<Uint8>(std::clamp(std::sin(colorTime) * Amplitude + MidPoint, 0.0f, 255.0f));
			g = static_cast<Uint8>(std::clamp(std::sin(colorTime + 2.0f) * Amplitude + MidPoint, 0.0f, 255.0f));
			b = static_cast<Uint8>(std::clamp(std::sin(colorTime + 4.0f) * Amplitude + MidPoint, 0.0f, 255.0f));
		}

		void RenderAnimation() const
		{
			if (spritesheet != nullptr)
				{
					const SDL_FRect src = {
							static_cast<float>(currentCol * TILE_SIZE),
							static_cast<float>(currentRow * TILE_SIZE),
							static_cast<float>(TILE_SIZE),
							static_cast<float>(TILE_SIZE),
					};

					constexpr SDL_FRect dst = {
							(800.0f / 2.0f) - ((TILE_SIZE * PRESENT_SIZE) / 2.0f),
							(600.0f / 2.0f) - ((TILE_SIZE * PRESENT_SIZE) / 2.0f),
							static_cast<float>((TILE_SIZE * PRESENT_SIZE)),
							static_cast<float>((TILE_SIZE * PRESENT_SIZE)),
					};

					SDL_RenderTexture(renderer, spritesheet, &src, &dst);
				}
		}

		void Run()
		{
			bool running = true;
			uint64_t lastTime = SDL_GetTicks();

			while (running)
				{
					SDL_Event event;
					while (SDL_PollEvent(&event))
						{
							if (event.type == SDL_EVENT_QUIT)
								running = false;
						}

					const uint64_t currentTime = SDL_GetTicks();
					const float deltaTime = static_cast<float>(currentTime - lastTime) / 1000.0f;
					lastTime = currentTime;
					CalculateFPS(deltaTime);

					AdvanceAnimation(deltaTime);
					UpdateBackgroundTint(deltaTime);

					SDL_SetRenderDrawColor(renderer, r, g, b, 255);
					SDL_RenderClear(renderer);

					RenderAnimation();
					TTF_DrawRendererText(fpsText, 10, 10);

					SDL_RenderPresent(renderer);
				}
		}
};

int main(int argc, char *argv[])
{

	GameApp app;

	app.Run();

	return 0;
}
