// Noise.cpp
// compile with: cl /c /EHsc Noise.cpp
// Noise generator for C++
// Includes implementation of Simplex and Perlin noise
// Manfred Hästmark 2023
// https://github.com/Manfred-Hastmark/Noise
//
// Perlin noise method is based on original implementation by Ken Perlin
// https://mrl.cs.nyu.edu/~perlin/noise/
//
// Simplex noise method is based on implementation by Stefan Gustavsson
// https://weber.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf
//

#include "Noise.h"

static const int p[512] = { 151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

namespace Noise
{
	using namespace Details;

	float Perlin::calculateNoise(float x)
	{
		//Find unit interval that contains x
		int X = (int)floor(x) & 255;

		//Find relative point in this interval
		x -= floor(x);

		//Compute fade curve
		float u = fade(x);

		//Add blended results from edges of interval
		return lerp(u, grad(p[X], x), grad(p[X + 1], x - 1));
	}

	float Perlin::calculateNoise(float x, float y)
	{
		//Find unit square that contains x, y
		int X = (int)floor(x) & 255;
		int Y = (int)floor(y) & 255;

		//Find relative point in this square
		x -= floor(x);
		y -= floor(y);

		//Compute fade curves for each x, y
		float u = fade(x);
		float v = fade(y);

		//Hash coordinates for each of the four corners
		int A = p[X] + Y & 255;
		int B = p[X + 1] + Y & 255;

		//Add blended results from 4 corners of square
		return lerp(v, lerp(u, grad(p[A], x, y),
			grad(p[B], x - 1, y)),
			lerp(u, grad(p[A + 1], x, y - 1),
				grad(p[B + 1], x - 1, y - 1)));
	}

	float Perlin::calculateNoise(float x, float y, float z)
	{
		//Find unit cube that contains x, y, z
		int X = (int)floor(x) & 255;
		int Y = (int)floor(y) & 255;
		int Z = (int)floor(z) & 255;

		//Find relative point on this cube
		x -= floor(x);
		y -= floor(y);
		z -= floor(z);

		//Compute fade curves for each x, y, z
		float u = fade(x);
		float v = fade(y);
		float w = fade(z);

		//Hash coordinates of each of the 8 cube corners
		int A = p[X] + Y & 255;
		int B = p[X + 1] + Y & 255;
		int AA = p[A] + Z & 255;
		int AB = p[A + 1] + Z & 255;
		int BA = p[B] + Z & 255;
		int BB = p[B + 1] + Z & 255;

		return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),                // Add
			grad(p[BA], x - 1, y, z)),           // blended
			lerp(u, grad(p[AB], x, y - 1, z),            // results
				grad(p[BB], x - 1, y - 1, z))),      // from 8
			lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),        // corners
				grad(p[BA + 1], x - 1, y, z - 1)),   // of cube
				lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
					grad(p[BB + 1], x - 1, y - 1, z - 1))));
	}

	float Simplex::calculateNoise(float x) { return 0; }

	float Simplex::calculateNoise(float x, float y) 
	{
		float n0, n1, n2; // Noise contributions from the three corners

		// Skew the input to the simplex grid and calculate which cell we are in (i, j)
		const float f2 = 0.5 * (sqrt(3.0) - 1.0);
		float s = (x + y) * f2; 
		int i = floor(x + s);	
		int j = floor(y + s);

		const float g2 = (3.0 - sqrt(3.0)) / 6.0;
		float t = (i + j) * g2;
		float X0 = i - t; // Unskew the cell origin back to (x,y) space
		float Y0 = j - t;
		float x0 = x - X0; // The x,y distances from the cell origin
		float y0 = y - Y0;

		// Determine which simplex we are in (upper or lower)
		int i1, j1;
		if (x0 > y0)
		{
			i1 = 1;	// lower triangle, XY order: (0,0)->(1,0)->(1,1)
			j1 = 0;
		}
		else
		{
			i1 = 0;	// upper triangle, YX order: (0,0)->(0,1)->(1,1)
			j1 = 1;
		}

		float x1 = x0 - i1 + g2; // Offsets for middle corner in (x,y) unskewed coords
		float y1 = y0 - j1 + g2;
		float x2 = x0 - 1.0 + 2.0 * g2; // Offsets for last corner in (x,y) unskewed coords
		float y2 = y0 - 1.0 + 2.0 * g2;

		// Work out the hashed gradient indices of the three simplex corners
		int ii = i & 255;
		int jj = j & 255;
		int gi0 = p[ii + p[jj]] & 255;
		int gi1 = p[ii + i1 + p[jj + j1]] & 255;
		int gi2 = p[ii + 1 + p[jj + 1]] & 255;

		// Calculate the contribution from the three corners
		float t0 = 0.5 - x0 * x0 - y0 * y0;
		if (t0 < 0) n0 = 0.0;
		else {
			t0 *= t0;
			n0 = t0 * t0 * grad(gi0, x0, y0); // (x,y) of grad3 used for 2D gradient
		}

		float t1 = 0.5 - x1 * x1 - y1 * y1;
		if (t1 < 0) n1 = 0.0;
		else {
			t1 *= t1;
			n1 = t1 * t1 * grad(gi1, x1, y1);
		}

		double t2 = 0.5 - x2 * x2 - y2 * y2;
		if (t2 < 0) n2 = 0.0;
		else {
			t2 *= t2;
			n2 = t2 * t2 * grad(gi2, x2, y2);
		}

		return 70.0 * (n0 + n1 + n2);
	}

	float Simplex::calculateNoise(float x, float y, float z) { return 0; }

	float Details::lerp(float t, float a, float b)
	{
		return a + t * (b - a);
	}

	float Details::fade(float t)
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	float Details::grad(int hash, float x)
	{
		return (hash & 1) == 0 ? x : -x;	//Let LSB represent direction vector -1 or 1
	}

	//TODO: Improve the hash of this function to include the 8 recomended vectors
	float Details::grad(int hash, float x, float y)
	{																	//Let bit0 represent direction vectors (1, 0) and (-1, 0)
		return ((hash & 1) == 0 ? x : -x) + ((hash & 2) == 0 ? y : -y);	//Let bit1 represent direction vectors (0, 1) and (0, -1)
	}

	float Details::grad(int hash, float x, float y, float z)
	{
		int h = hash & 15;                      // Convert lo 4 bits of hash code
		float u = h < 8 ? x : y;                 // into 12 gradient vectors.
		float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
		return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	}
}

