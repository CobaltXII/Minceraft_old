// Modified version of https://github.com/Reputeless/PerlinNoise.

#include <algorithm>
#include <random>

struct Perlin
{
	// Permutation.

	int p[512];

	// Ease curve or Fade function.

	static inline double Fade(double t)
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	// Interpolate.

	static inline double Lerp(double t, double a, double b)
	{
		return a + t * (b - a);
	}

