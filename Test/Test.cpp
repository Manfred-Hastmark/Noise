// test.cpp


#include "Test.h"
#include "SDL.h"
#include "Noise.h"

#include <iostream>

int main(int argc, char* argv[])
{
	while (1) 
	{
		std::cout << "Please enter 1,2 or 3 to showcase the different Perlin noises: ";

		int choice;
		std::cin >> choice;

		switch (choice)
		{
		case Choice::perlin1D:
			show1dPerlinNoise();
			break;
		case Choice::perlin2D:
			show2dPerlinNoise();
			break;
		case Choice::perlin3D:
			show3dPerlinNoise();
			break;
		default:
			std::cout << "Please enter a valid choice\n";
			break;
		}
	}
	return 0;
}


/*
* Graphs a line using 1D perlin noise
*/
void show1dPerlinNoise()
{
	std::cout << "Please enter frequency: ";

	float frequency;
	std::cin >> frequency;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	float perlin = (Noise::Perlin::calculateNoise(0) + 1) / 2;
	int y = (int)(perlin * WINDOW_HEIGHT);

	for (int x = 0; x < WINDOW_WIDTH; x++)
	{
		float xRel = (x * frequency) / WINDOW_WIDTH;
		perlin = (Noise::Perlin::calculateNoise(xRel) + 1) / 2;
		SDL_RenderDrawLine(renderer, x - 1, y, x, (int)(perlin * WINDOW_HEIGHT));
		y = (int)(perlin * WINDOW_HEIGHT);
	}
	
	SDL_RenderPresent(renderer);
	SDL_Delay(3000);
	SDL_DestroyWindow(window);
}

/*
* Draws a heightmap lookalike using 2D perlin noise
*/
void show2dPerlinNoise()
{
	std::cout << "Please enter frequency: ";

	float frequency;
	std::cin >> frequency;

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
	SDL_DestroyWindow(window);
}


void show3dPerlinNoise()
{
	std::cout << "Please enter frequency: ";

	float frequency;
	std::cin >> frequency;
}