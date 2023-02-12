#pragma once

#define WINDOW_WIDTH 1080
#define	WINDOW_HEIGHT 610


enum Choice {perlin1D = 1, perlin2D = 2, perlin3D = 3};

void show1dPerlinNoise();
void show2dPerlinNoise();
void show3dPerlinNoise();
void getInput(int* FPS, float* frequency, int* r, int* g, int* b);