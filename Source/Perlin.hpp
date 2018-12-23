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

	// Gradient function.

	static inline double Grad(int hash, double x, double y, double z)
	{
		int h = hash & 15;

		double u;
		double v;

		if (h < 8)
		{
			u = x;
		}
		else
		{
			u = y;
		}

		if (h < 4)
		{
			v = y;
		}
		else
		{
			if (h == 12 || h == 14)
			{
				v = x;
			}
			else
			{
				v = z;
			}
		}

		double a;
		double b;

		if ((h & 1) == 0)
		{
			a = u;
		}
		else
		{
			a = -u;
		}

		if ((h & 2) == 0)
		{
			b = v;
		}
		else
		{
			b = -v;
		}

		return a + b;
	}

