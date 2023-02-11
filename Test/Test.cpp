// test.cpp

#include "SDL.h"

#include "Noise.h"

#define WINDOW_WIDTH 600
#define	WINDOW_HEIGHT 400

int main(int argc, char* argv[])
{
	const float frequency = 5;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	for (int x = 0; x < WINDOW_WIDTH; x++)
	{
		for (int y = 0; y < WINDOW_HEIGHT; y++)
		{
			float xRel = (x * frequency) / WINDOW_WIDTH;
			float yRel = (y * frequency) / WINDOW_HEIGHT;
			float perlin = (Noise::Perlin::calculateNoise(xRel, yRel) + 1) / 2;
			SDL_SetRenderDrawColor(renderer, perlin * 255, perlin * 255, perlin * 255, 255);
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}

	SDL_RenderPresent(renderer);

	SDL_Delay(3000);

	return 0;
}