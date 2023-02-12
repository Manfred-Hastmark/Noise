// test.cpp


#include "Test.h"
#include "SDL.h"
#include "Noise.h"
#include "Particle.h"

#include <math.h>
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

	// Close window when the "x" is pressed
	SDL_Event e;
	while (1)
	{
		SDL_WaitEvent(&e);
		if (e.type == SDL_QUIT)
		{
			SDL_DestroyWindow(window);
			break;
		}
	}
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

	
	// Close window when the "x" is pressed
	SDL_Event e;
	while (1)
	{
		SDL_WaitEvent(&e);
		if (e.type == SDL_QUIT)
		{
			SDL_DestroyWindow(window);
			break;
		}
	}
}


void show3dPerlinNoise()
{
	const int noParticles = 10000;
	int FPS;
	float frequency;
	int r, g, b;

	getInput(&FPS, &frequency, &r, &g, &b);

	//Allocate particles array on the heap
	Particle* particles = new Particle[noParticles];

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(renderer, r, g, b, 1);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
	//initialize the particles
	for (int i = 0; i < noParticles; i++)
	{
		particles[i] = *(new Particle(WINDOW_WIDTH, WINDOW_HEIGHT, 10));
	}


	int deltaTime = 0;
	SDL_Event e;

	while (1)
	{
		if(SDL_PollEvent(&e) && e.type == SDL_QUIT)
			break;

		//Update and redraw each point
		for (int i = 0; i < noParticles; i++)
		{
			particles[i].update();

			float xRel = (particles[i].x * frequency) / WINDOW_WIDTH;
			float yRel = (particles[i].y * frequency) / WINDOW_HEIGHT;

			float angle = (Noise::Perlin::calculateNoise(xRel, yRel, SDL_GetTicks() * 0.001) + 1) * M_PI;
			particles[i].addForce(1, angle);

			SDL_RenderDrawLine(renderer, particles[i].x, particles[i].y, particles[i].prevX, particles[i].prevY);
		}

		//Slowdown to the wanted FPS
		while (SDL_GetTicks() - deltaTime < (1000 / FPS));
		deltaTime = SDL_GetTicks();

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
}

void getInput(int* FPS, float* frequency, int* r, int* g, int* b)
{
	//std::cout << "Please enter number of particles to be used: ";
	//std::cin >> *noParticles;

	std::cout << "Please enter the FPS limit to be used: ";
	std::cin >> *FPS;

	std::cout << "Please enter the freuqency of the noise to be used: ";
	std::cin >> *frequency;

	std::cout << "Please enter how much red should be used (0-255): ";
	std::cin >> *r;

	std::cout << "Please enter how much green should be used (0-255): ";
	std::cin >> *g;

	std::cout << "Please enter how much blue should be used (0-255): ";
	std::cin >> *b;
}