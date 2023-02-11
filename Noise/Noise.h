// Noise.h
#pragma once


namespace Noise
{
	class Perlin
	{
	public:
		// Returns noise value for given position x
		static float calculateNoise(float x);

		// Returns noise value for given position x, y
		static float calculateNoise(float x, float y);

		// Returns noise value for given position x,y,z
		static float calculateNoise(float x, float y, float z);

	private:
		// Interpolates from point a to b using t
		static float lerp(float t, float a, float b);

		// Calculate f(t) = 6*t^5 - 15*t^4 + 10*t^3
		static float fade(float t);

		// Calculates the dot product between gradient retrieved from hash and x
		static float grad(int hash, float x);

		// Calculates the dot product between gradient retrieved from hash and x, y
		static float grad(int hash, float x, float y);

		// Calculates the dot product between gradient retrieved from hash and x, y, z
		static float grad(int hash, float x, float y, float z);
	};
}