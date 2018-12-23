// Modified version of https://github.com/Reputeless/PerlinNoise.

#include <algorithm>
#include <random>

struct Perlin
{
	// Permutation.

	int p[512];

