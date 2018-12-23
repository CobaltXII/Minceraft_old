#include <iostream>

// A world can be represented using a World type. A block can be represented using a Voxel type.
// The world typename Voxel is used to avoid confusion with Block_ID.

typedef unsigned short Voxel;

// Get the Block_ID of a voxel.

inline Block_ID Voxel_Type(Voxel Which)
{
	return Block_ID(Which & 0xFF);
}

// Get the skylight contribution of a voxel.

inline unsigned char Voxel_Skylight(Voxel Which)
{
	return (Which >> 12); 
}

// Get the artificial light contribution of a voxel.

inline unsigned char Voxel_Light(Voxel Which)
{
	return (Which >> 8) & 0xF; 
}

// Create a voxel given a Block_ID, a skylight contibution, and an artificial light contibution.

inline Voxel Make_Voxel(unsigned short Type, unsigned short Skylight, unsigned short Light)
{
	return (Skylight << 12) | (Light << 8) | Type; 
}

inline Voxel Make_Voxel(unsigned short Type)
{
	return 0xFF00 | Type; 
}

struct World
{
	Voxel* Voxels;

	unsigned int X_Res;
	unsigned int Z_Res;
	unsigned int Y_Res;

	// Get the value of a block in the world.

	inline Voxel Get(unsigned int X, unsigned int Y, unsigned int Z)
	{
		return Voxels[X + X_Res * (Y + Y_Res * Z)];
	}

	// Get the value of a block in the world.

	inline Voxel Get_Safe(unsigned int X, unsigned int Y, unsigned int Z)
	{
		if (X < 0 || Y < 0 || Z < 0 || X > X_Res - 1 || Y > Y_Res - 1 || Z > Z_Res - 1)
		{
			return Make_Voxel(id_air);
		}

		return Voxels[X + X_Res * (Y + Y_Res * Z)];
	}

	// Set the value of a block in the world.

	inline void Set(unsigned int X, unsigned int Y, unsigned int Z, Voxel Value)
	{
		Voxels[X + X_Res * (Y + Y_Res * Z)] = Value;
	}

	// Set the value of a block in the world.

	inline void Set_Safe(unsigned int X, unsigned int Y, unsigned int Z, Voxel Value)
	{
		if (X < 0 || Y < 0 || Z < 0 || X > X_Res - 1 || Y > Y_Res - 1 || Z > Z_Res - 1)
		{
			return;
		}

		Voxels[X + X_Res * (Y + Y_Res * Z)] = Value;
	}

	// Check if a point is out of bounds.

	inline bool Out_Of_Bounds(unsigned int X, unsigned int Y, unsigned int Z)
	{
		return X < 0 || Y < 0 || Z < 0 || X > X_Res - 1 || Y > Y_Res - 1 || Z > Z_Res - 1;
	}
};

// Tiny world.

unsigned int Tiny_World_X_Res = 64;

unsigned int Tiny_World_Y_Res = 32;

unsigned int Tiny_World_Z_Res = 64;

unsigned int Tiny_World_Voxels = Tiny_World_X_Res * Tiny_World_Y_Res * Tiny_World_Z_Res;

unsigned int Tiny_World_Memory = sizeof(Voxel) * Tiny_World_Voxels;

// Small world.

unsigned int Small_World_X_Res = 128;

unsigned int Small_World_Y_Res = 64;

unsigned int Small_World_Z_Res = 128;

unsigned int Small_World_Voxels = Small_World_X_Res * Small_World_Y_Res * Small_World_Z_Res;

unsigned int Small_World_Memory = sizeof(Voxel) * Small_World_Voxels;

// Medium world.

unsigned int Medium_World_X_Res = 256;

unsigned int Medium_World_Y_Res = 128;

unsigned int Medium_World_Z_Res = 256;

unsigned int Medium_World_Voxels = Medium_World_X_Res * Medium_World_Y_Res * Medium_World_Z_Res;

unsigned int Medium_World_Memory = sizeof(Voxel) * Medium_World_Voxels;

// Large world.

unsigned int Large_World_X_Res = 512;

unsigned int Large_World_Y_Res = 256;

unsigned int Large_World_Z_Res = 512;

unsigned int Large_World_Voxels = Large_World_X_Res * Large_World_Y_Res * Large_World_Z_Res;

unsigned int Large_World_Memory = sizeof(Voxel) * Large_World_Voxels;

// Huge world.

unsigned int Huge_World_X_Res = 1024;

unsigned int Huge_World_Y_Res = 256;

unsigned int Huge_World_Z_Res = 1024;

unsigned int Huge_World_Voxels = Huge_World_X_Res * Huge_World_Y_Res * Huge_World_Z_Res;

unsigned int Huge_World_Memory = sizeof(Voxel) * Huge_World_Voxels;

// World resolutions.

unsigned int World_X_Res_List[5] = {Tiny_World_X_Res, Small_World_X_Res, Medium_World_X_Res, Large_World_X_Res, Huge_World_X_Res};
unsigned int World_Y_Res_List[5] = {Tiny_World_Y_Res, Small_World_Y_Res, Medium_World_Y_Res, Large_World_Y_Res, Huge_World_Y_Res};
unsigned int World_Z_Res_List[5] = {Tiny_World_Z_Res, Small_World_Z_Res, Medium_World_Z_Res, Large_World_Z_Res, Huge_World_Z_Res};

const int Id_Tiny_World = 0;

const int Id_Small_World = 1;

const int Id_Medium_World = 2;

const int Id_Large_World = 3;

const int Id_Huge_World = 4;

// Allocate memory for an empty world with the specified dimensions (use the world size
// constants).

World* Allocate_Empty_World(unsigned int X_Res, unsigned int Y_Res, unsigned int Z_Res)
{
	World* New_World = new World();

	New_World->Voxels = (Voxel*)malloc(X_Res * Y_Res * Z_Res * sizeof(Voxel));

	if (!New_World->Voxels)
	{
		std::cout << "Could not allocate space to hold a World object (" << (X_Res * Y_Res * Z_Res * sizeof(Voxel)) << " bytes are needed)." << std::endl;

		exit(11);
	}

	memset(New_World->Voxels, 0, X_Res * Y_Res * Z_Res * sizeof(Voxel));

	New_World->X_Res = X_Res;
	New_World->Y_Res = Y_Res;
	New_World->Z_Res = Z_Res;

	return New_World;
}

// Destroy a world!

void Destroy_World(World* To_Be_Annihilated)
{
	free(To_Be_Annihilated->Voxels);
}

// Use the standard world generation algorithm to generate a world.

void Generate_World(World* Out, unsigned int Seed)
{
	// We will use rand() to get pure random noise, and use a Perlin object to get smooth noise
	// for terrain.

	srand(Seed);

	Perlin Noise = Perlin(Seed);

	// Generate the base terrain. Basically, this uses 3-dimensional Perlin noise to generate a
	// terrain-like shape. Anything that is inside this shape is set to stone.

	float Frequency = 4.0f;

	for (float X = 0; X < Out->X_Res; X++)
	{
		for (float Y = 0; Y < Out->Y_Res; Y++)
		{
			for (float Z = 0; Z < Out->Z_Res; Z++)
			{
				float Xf = X / float(Out->X_Res);
				float Yf = Y / float(Out->Y_Res);
				float Zf = Z / float(Out->Z_Res);

				float Yn = Yf + Noise.Octave_Noise(Xf * Frequency, Yf * Frequency, Zf * Frequency, 4) * 0.6f;

				if (Yn >= 0.5f)
				{
					Out->Set(int(X), int(Y), int(Z), Make_Voxel(id_stone));
				}
			}
		}
	}

	// Set the highground blocks. Basically, this function finds the highest block for each X/Z
	// strip of the world. The highest block is set to grass if it is above a certain threshold.
	// If it is below, it is set to either sand or gravel. The three blocks below the highest
	// block are set to dirt.

	for (float X = 0; X < Out->X_Res; X++)
	{
		for (float Z = 0; Z < Out->Z_Res; Z++)
		{
			float Sand_Chance = Noise.Noise(X / float(Out->X_Res) * Frequency, Z / float(Out->Z_Res) * Frequency);

			for (float Y = 0; Y < Out->Y_Res; Y++)
			{
				if (Out->Get(int(X), int(Y), int(Z)) != id_air)
				{
					if (Y / float(Out->Y_Res) > 0.6f)
					{
						if (Sand_Chance >= -0.2f)
						{
							Out->Set_Safe(int(X), int(Y), int(Z), Make_Voxel(id_sand));
						}
						else
						{
							Out->Set_Safe(int(X), int(Y), int(Z), Make_Voxel(id_gravel));
						}
					}
					else
					{
						Out->Set_Safe(int(X), int(Y), int(Z), Make_Voxel(id_grass));
					}

					Out->Set_Safe(int(X), int(Y) + 1, int(Z), Make_Voxel(id_dirt));
					Out->Set_Safe(int(X), int(Y) + 2, int(Z), Make_Voxel(id_dirt));
					Out->Set_Safe(int(X), int(Y) + 3, int(Z), Make_Voxel(id_dirt));

					break;
				}
			}
		}
	}

	// Add some ores. Basically chooses a bunch of random spots, and gets all the points in a 
	// deformed sphere around that point. For each of those points that are stone, set them to the
	// ore.

	for (int I = 0; I < Out->X_Res * Out->Z_Res * Out->Y_Res / 1024; I++)
	{
		// Coal ore.

		int X = rand() % Out->X_Res;
		int Y = rand() % Out->Y_Res;
		int Z = rand() % Out->Z_Res;

		int Radius = rand() % 3 + 1;

		for (int Xr = -Radius; Xr <= Radius; Xr++)
		{
			for (int Yr = -Radius; Yr <= Radius; Yr++)
			{
				for (int Zr = -Radius; Zr <= Radius; Zr++)
				{
					if (Xr * Xr + Yr * Yr + Zr * Zr < Radius * Radius - (rand() % 2))
					{
						if (Voxel_Type(Out->Get_Safe(X + Xr, Y + Yr, Z + Zr)) == id_stone)
						{
							Out->Set_Safe(X + Xr, Y + Yr, Z + Zr, Make_Voxel(id_coal_ore));
						}
					}
				}
			}
		}
	}

	for (int I = 0; I < Out->X_Res * Out->Z_Res * Out->Y_Res / 2048; I++)
	{
		// Iron ore.

		int X = rand() % Out->X_Res;

		int Y = (rand() % int(float(Out->Y_Res) * 0.35)) + (float(Out->Y_Res) * 0.65);

		int Z = rand() % Out->Z_Res;

		int Radius = rand() % 2 + 1;

		for (int Xr = -Radius; Xr <= Radius; Xr++)
		{
			for (int Yr = -Radius; Yr <= Radius; Yr++)
			{
				for (int Zr = -Radius; Zr <= Radius; Zr++)
				{
					if (Xr * Xr + Yr * Yr + Zr * Zr < Radius * Radius - (rand() % 2))
					{
						if (Voxel_Type(Out->Get_Safe(X + Xr, Y + Yr, Z + Zr)) == id_stone)
						{
							Out->Set_Safe(X + Xr, Y + Yr, Z + Zr, Make_Voxel(id_iron_ore));
						}
					}
				}
			}
		}
	}

	for (int I = 0; I < Out->X_Res * Out->Z_Res * Out->Y_Res / 3072; I++)
	{
		// Gold ore.

		int X = rand() % Out->X_Res;

		int Y = (rand() % int(float(Out->Y_Res) * 0.25)) + (float(Out->Y_Res) * 0.75);

		int Z = rand() % Out->Z_Res;

		int Radius = rand() % 2 + 1;

		for (int Xr = -Radius; Xr <= Radius; Xr++)
		{
			for (int Yr = -Radius; Yr <= Radius; Yr++)
			{
				for (int Zr = -Radius; Zr <= Radius; Zr++)
				{
					if (Xr * Xr + Yr * Yr + Zr * Zr < Radius * Radius - (rand() % 2))
					{
						if (Voxel_Type(Out->Get_Safe(X + Xr, Y + Yr, Z + Zr)) == id_stone)
						{
							Out->Set_Safe(X + Xr, Y + Yr, Z + Zr, Make_Voxel(id_gold_ore));
						}
					}
				}
			}
		}
	}

