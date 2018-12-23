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

	// Three-dimensional noise.

	double Noise(double x, double y, double z)
	{
		int X = int(std::floor(x)) & 255;
		int Y = int(std::floor(y)) & 255;
		int Z = int(std::floor(z)) & 255;

		x -= std::floor(x);
		y -= std::floor(y);
		z -= std::floor(z);

		double u = Fade(x);
		double v = Fade(y);
		double w = Fade(z);

		int A = p[X] + Y;

		int B = p[X + 1] + Y;

		int AA = p[A] + Z;
		int BA = p[B] + Z;

		int AB = p[A + 1] + Z;
		int BB = p[B + 1] + Z;
		
		return Lerp(w, Lerp(v, Lerp(u, Grad(p[AA], x, y, z), Grad(p[BA], x - 1, y, z)), Lerp(u, Grad(p[AB], x, y - 1, z), Grad(p[BB], x - 1, y - 1, z))), Lerp(v, Lerp(u, Grad(p[AA + 1], x, y, z - 1), Grad(p[BA + 1], x - 1, y, z - 1)), Lerp(u, Grad(p[AB + 1], x, y - 1, z - 1), Grad(p[BB + 1], x - 1, y - 1, z - 1))));
	}

	// Two-dimensional noise.

	inline double Noise(double x, double y)
	{
		return Noise(x, y, 0.0);
	}

	// One-dimensional noise.

	inline double Noise(double x)
	{
		return Noise(x, 0.0, 0.0);
	}

	// Three-dimensional octave noise.

	double Octave_Noise(double x, double y, double z, int octaves)
	{
		double result = 0.0;

		double amp = 1.0;

		for (int i = 0; i < octaves; i++)
		{
			result += Noise(x, y, z) * amp;

			x *= 2.0;
			y *= 2.0;
			z *= 2.0;
			
			amp *= 0.5;
		}

		return result;
	}

	// Two-dimensional octave noise.

	double Octave_Noise(double x, double y, int octaves)
	{
		double result = 0.0;

		double amp = 1.0;

		for (int i = 0; i < octaves; i++)
		{
			result += Noise(x, y) * amp;

			x *= 2.0;
			y *= 2.0;

			amp *= 0.5;
		}

		return result;
	}

	// One-dimensional octave noise.

	double Octave_Noise(double x, int octaves)
	{
		double result = 0.0;

		double amp = 1.0;

		for (int i = 0; i < octaves; i++)
		{
			result += Noise(x) * amp;

			x *= 2.0;

			amp *= 0.5;
		}

		return result;
	}

	// Scaled one-dimensional noise.

	inline double Noise_0_1(double x)
	{
		return Noise(x) * 0.5 + 0.5;
	}

	// Scaled two-dimensional noise.

	inline double Noise_0_1(double x, double y)
	{
		return Noise(x, y) * 0.5 + 0.5;
	}

	// Scaled three-dimensional noise.

	inline double Noise_0_1(double x, double y, double z)
	{
		return Noise(x, y, z) * 0.5 + 0.5;
	}

	// Scaled one-dimensional octave noise.

	inline double Octave_Noise_0_1(double x, int octaves)
	{
		return Octave_Noise(x, octaves) * 0.5 + 0.5;
	}

	// Scaled two-dimensional octave noise.

	inline double Octave_Noise_0_1(double x, double y, int octaves)
	{
		return Octave_Noise(x, y, octaves) * 0.5 + 0.5;
	}

	// Scaled three-dimensional octave noise.

	inline double Octave_Noise_0_1(double x, double y, double z, int octaves)
	{
		return Octave_Noise(x, y, z, octaves) * 0.5 + 0.5;
	}

