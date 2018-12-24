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

	// Set the value of a block in the world, if the type of the current block at the same 
	// position is air.

	inline void Set_Safe_If_Air(unsigned int X, unsigned int Y, unsigned int Z, Voxel Value)
	{
		if (X < 0 || Y < 0 || Z < 0 || X > X_Res - 1 || Y > Y_Res - 1 || Z > Z_Res - 1)
		{
			return;
		}

		if (Voxel_Type(Voxels[X + X_Res * (Y + Y_Res * Z)]) == id_air)
		{
			Voxels[X + X_Res * (Y + Y_Res * Z)] = Value;
		}
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

	// Flood fill water in from the edges.

	for (int X = 0; X < Out->X_Res; X++)
	{
		for (int Y = 0; Y < Out->Y_Res; Y++)
		{
			for (int Z = 0; Z < Out->Z_Res; Z++)
			{
				if (float(Y) / float(Out->Y_Res) > 0.5f)
				{
					if (Voxel_Type(Out->Get(X, Y, Z)) == id_air)
					{
						Out->Set(X, Y, Z, Make_Voxel(id_water));
					}
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

	for (int I = 0; I < Out->X_Res * Out->Z_Res * Out->Y_Res / 16384; I++)
	{
		// Diamond ore.

		int X = rand() % Out->X_Res;

		int Y = (rand() % int(float(Out->Y_Res) * 0.15)) + (float(Out->Y_Res) * 0.85);

		int Z = rand() % Out->Z_Res;

		int Radius = 1;

		for (int Xr = -Radius; Xr <= Radius; Xr++)
		{
			for (int Yr = -Radius; Yr <= Radius; Yr++)
			{
				for (int Zr = -Radius; Zr <= Radius; Zr++)
				{
					if (Xr * Xr + Yr * Yr + Zr * Zr < Radius * Radius + 1)
					{
						if (Voxel_Type(Out->Get_Safe(X + Xr, Y + Yr, Z + Zr)) == id_stone)
						{
							Out->Set_Safe(X + Xr, Y + Yr, Z + Zr, Make_Voxel(id_diamond_ore));
						}
					}
				}
			}
		}
	}

	// Plant trees.

	for (int I = 0; I < Out->X_Res * Out->Z_Res / 32; I++)
	{
		Tree:

		int X = rand() % Out->X_Res;
		int Z = rand() % Out->Z_Res;

		Block_ID Tree_Leaf;

		Block_ID Tree_Log;

		if (rand() % 3 == 0)
		{
			Tree_Leaf = id_birch_leaves;

			Tree_Log = id_birch_log;
		}
		else
		{
			Tree_Leaf = id_oak_leaves;

			Tree_Log = id_oak_log;
		}

		// Find the highest layer.

		for (float Y = 0; Y < Out->Y_Res; Y++)
		{
			if (Voxel_Type(Out->Get(int(X), int(Y), int(Z))) != id_air)
			{
				// Okay, found something that is not air.

				if (Voxel_Type(Out->Get(int(X), int(Y), int(Z))) == id_grass)
				{
					// It's grass, we may plant a tree if there are enough blocks available above 
					// the grass.

					if (rand() % 2 == 0)
					{
						// Original tree.

						if (Out->Out_Of_Bounds(X, Y - 6, Z))
						{
							goto Tree;
						} 

						// This is the trunk.

						Out->Set_Safe(X, Y - 1, Z, Make_Voxel(Tree_Log));
						Out->Set_Safe(X, Y - 2, Z, Make_Voxel(Tree_Log));
						Out->Set_Safe(X, Y - 3, Z, Make_Voxel(Tree_Log));
						Out->Set_Safe(X, Y - 4, Z, Make_Voxel(Tree_Log));
						Out->Set_Safe(X, Y - 5, Z, Make_Voxel(Tree_Log));

						// This is the cross at the top.

						Out->Set_Safe(X, Y - 6, Z, Make_Voxel(Tree_Leaf));

						Out->Set_Safe(X - 1, Y - 6, Z, Make_Voxel(Tree_Leaf));
						Out->Set_Safe(X + 1, Y - 6, Z, Make_Voxel(Tree_Leaf));

						Out->Set_Safe(X, Y - 6, Z + 1, Make_Voxel(Tree_Leaf));
						Out->Set_Safe(X, Y - 6, Z - 1, Make_Voxel(Tree_Leaf));

						// This is the square at the layer second from the top.

						for (int J = -1; J <= 1; J++)
						{
							for (int K = -1; K <= 1; K++)
							{
								if (J == 0 && K == 0)
								{
									continue;
								}
								
								Out->Set_Safe(X + J, Y - 5, Z + K, Make_Voxel(Tree_Leaf));
							}
						}

						// These are the squares at the third and fourth layer from the top.

						for (int J = -2; J <= 2; J++)
						{
							for (int K = -2; K <= 2; K++)
							{
								if (J == 0 && K == 0)
								{
									continue;
								}
								
								Out->Set_Safe(X + J, Y - 4, Z + K, Make_Voxel(Tree_Leaf));
								Out->Set_Safe(X + J, Y - 3, Z + K, Make_Voxel(Tree_Leaf));
							}
						}
					}
					else
					{
						// Rounded tree.

						if (Out->Out_Of_Bounds(X, Y - 7, Z))
						{
							goto Tree;
						} 

						// This is the trunk.

						Out->Set_Safe(X, Y - 1, Z, Make_Voxel(Tree_Log));
						Out->Set_Safe(X, Y - 2, Z, Make_Voxel(Tree_Log));
						Out->Set_Safe(X, Y - 3, Z, Make_Voxel(Tree_Log));
						Out->Set_Safe(X, Y - 4, Z, Make_Voxel(Tree_Log));
						Out->Set_Safe(X, Y - 5, Z, Make_Voxel(Tree_Log));
						Out->Set_Safe(X, Y - 6, Z, Make_Voxel(Tree_Log));

						// This is the cross at the top.

						Out->Set_Safe(X, Y - 7, Z, Make_Voxel(Tree_Leaf));

						Out->Set_Safe(X - 1, Y - 7, Z, Make_Voxel(Tree_Leaf));
						Out->Set_Safe(X + 1, Y - 7, Z, Make_Voxel(Tree_Leaf));

						Out->Set_Safe(X, Y - 7, Z + 1, Make_Voxel(Tree_Leaf));
						Out->Set_Safe(X, Y - 7, Z - 1, Make_Voxel(Tree_Leaf));

						// This is the square at the layer second from the top.

						for (int J = -1; J <= 1; J++)
						{
							for (int K = -1; K <= 1; K++)
							{
								if (J == 0 && K == 0)
								{
									continue;
								}
								
								Out->Set_Safe(X + J, Y - 6, Z + K, Make_Voxel(Tree_Leaf));
							}
						}

						// These are the squares at the third and fourth layer from the top.

						for (int J = -2; J <= 2; J++)
						{
							for (int K = -2; K <= 2; K++)
							{
								if (J == 0 && K == 0)
								{
									continue;
								}
								
								Out->Set_Safe(X + J, Y - 5, Z + K, Make_Voxel(Tree_Leaf));
								Out->Set_Safe(X + J, Y - 4, Z + K, Make_Voxel(Tree_Leaf));
							}
						}

						// This is the square at the fifth layer from the top. It has it's corners
						// removed.

						for (int J = -2; J <= 2; J++)
						{
							for (int K = -2; K <= 2; K++)
							{
								if (J == 0 && K == 0)
								{
									continue;
								}
								else if ((J == 2 && K == 2) || (J == 2 && K == -2) || (J == -2 && K == 2) || (J == -2 && K == -2))
								{
									continue;
								}
								
								Out->Set_Safe(X + J, Y - 3, Z + K, Make_Voxel(Tree_Leaf));
							}
						}
					}
				}

				break;
			}
		}
	}

	// Plant mushrooms.

	for (int I = 0; I < Out->X_Res * Out->Z_Res / 1024; I++)
	{
		Mushroom:

		int X = rand() % Out->X_Res;
		int Z = rand() % Out->Z_Res;

		for (float Y = 0; Y < Out->Y_Res; Y++)
		{
			if (Voxel_Type(Out->Get(int(X), int(Y), int(Z))) != id_air)
			{
				// Okay, found something that is not air.

				if (Voxel_Type(Out->Get(int(X), int(Y), int(Z))) == id_grass)
				{
					// It's grass, we may plant a mushroom if there are enough blocks available 
					// above the grass.

					if (Out->Out_Of_Bounds(X, Y - 6, Z))
					{
						goto Mushroom;
					}

					if (rand() % 5 != 0)
					{
						// Red mushroom. This is the trunk.

						Out->Set_Safe(X, Y - 1, Z, Make_Voxel(id_mushroom_stem));
						Out->Set_Safe(X, Y - 2, Z, Make_Voxel(id_mushroom_stem));
						Out->Set_Safe(X, Y - 3, Z, Make_Voxel(id_mushroom_stem));
						Out->Set_Safe(X, Y - 4, Z, Make_Voxel(id_mushroom_stem));
						Out->Set_Safe(X, Y - 5, Z, Make_Voxel(id_mushroom_stem));

						// This is the square at the top.

						for (int J = -1; J <= 1; J++)
						{
							for (int K = -1; K <= 1; K++)
							{
								Out->Set_Safe(X + J, Y - 6, Z + K, Make_Voxel(id_red_mushroom_block));
							}
						}

						// This is the square on the left.

						for (int J = -1; J <= 1; J++)
						{
							for (int K = -1; K <= 1; K++)
							{
								Out->Set_Safe(X - 2, Y - 4 + J, Z + K, Make_Voxel(id_red_mushroom_block));

								Out->Set_Safe(X - 1, Y - 4 + J, Z + K, Make_Voxel(id_air));
							}
						}

						// This is the square on the right.

						for (int J = -1; J <= 1; J++)
						{
							for (int K = -1; K <= 1; K++)
							{
								Out->Set_Safe(X + 2, Y - 4 + J, Z + K, Make_Voxel(id_red_mushroom_block));

								Out->Set_Safe(X + 1, Y - 4 + J, Z + K, Make_Voxel(id_air));
							}
						}

						// This is the square on the front.

						for (int J = -1; J <= 1; J++)
						{
							for (int K = -1; K <= 1; K++)
							{
								Out->Set_Safe(X + K, Y - 4 + J, Z - 2, Make_Voxel(id_red_mushroom_block));

								Out->Set_Safe(X + K, Y - 4 + J, Z - 1, Make_Voxel(id_air));
							}
						}

						// This is the square on the back.

						for (int J = -1; J <= 1; J++)
						{
							for (int K = -1; K <= 1; K++)
							{
								Out->Set_Safe(X + K, Y - 4 + J, Z + 2, Make_Voxel(id_red_mushroom_block));

								Out->Set_Safe(X + K, Y - 4 + J, Z + 1, Make_Voxel(id_air));
							}
						}
					}
					else
					{
						// Brown mushroom. This is the trunk.

						Out->Set_Safe(X, Y - 1, Z, Make_Voxel(id_mushroom_stem));
						Out->Set_Safe(X, Y - 2, Z, Make_Voxel(id_mushroom_stem));
						Out->Set_Safe(X, Y - 3, Z, Make_Voxel(id_mushroom_stem));
						Out->Set_Safe(X, Y - 4, Z, Make_Voxel(id_mushroom_stem));
						Out->Set_Safe(X, Y - 5, Z, Make_Voxel(id_mushroom_stem));

						for (int J = -3; J <= 3; J++)
						{
							for (int K = -3; K <= 3; K++)
							{
								if ((J == 3 && K == 3) || (J == 3 && K == -3) || (J == -3 && K == 3) || (J == -3 && K == -3))
								{
									continue;
								}

								Out->Set_Safe(X + J, Y - 6, Z + K, Make_Voxel(id_brown_mushroom_block));
							}
						}
					}
				}
			}
		}
	}

	// Add bedrock. This will fill the bottom layer with 100% bedrock, and the second from bottom
	// layer with ~50% bedrock.

	for (float X = 0; X < Out->X_Res; X++)
	{
		for (float Z = 0; Z < Out->Z_Res; Z++)
		{
			Out->Set(X, Out->Y_Res - 1, Z, Make_Voxel(id_bedrock));

			if (rand() % 2 == 0)
			{
				Out->Set(X, Out->Y_Res - 2, Z, Make_Voxel(id_bedrock));
			}
		}
	}

	// Calculate the lighting of each block. Currently, the highest block in each X/Z stripe is
	// set to full light (15), and everything below is set to medium light (7).

	for (float X = 0; X < Out->X_Res; X++)
	{
		for (float Z = 0; Z < Out->Z_Res; Z++)
		{
			unsigned char Skylight = 15;

			for (float Y = 0; Y < Out->Y_Res; Y++)
			{
				Block_ID Block_Type = Voxel_Type(Out->Get(int(X), int(Y), int(Z)));

				Out->Set(int(X), int(Y), int(Z), Make_Voxel(Block_Type, Skylight, 0));

				if (Block_Type != id_air)
				{
					if (Skylight != 0)
					{
						Skylight -= 1;
					}
				}
			}
		}
	}

	// World generation is complete. This comment is used to mark the end of the function. It is
	// hard to float in curly-brace soup.
}

// Propagate skylight throughout a subset of the world.

void Propagate_Skylight
(
	World* Input, 

	unsigned int X, 
	unsigned int Y, 
	unsigned int Z, 

	unsigned int X_Res, 
	unsigned int Y_Res, 
	unsigned int Z_Res
)
{
	unsigned int Fx = X + X_Res;
	unsigned int Fy = Y + Y_Res;
	unsigned int Fz = Z + Z_Res;

	for (unsigned int Cx = X; Cx < Fx; Cx++)
	{
		for (unsigned int Cz = Z; Cz < Fz; Cz++)
		{
			unsigned int Skylight;

			if (Y == 0)
			{
				// Top of the world, where the sky is brightest.

				Skylight = 15;
			}
			else
			{
				Skylight = Voxel_Skylight(Input->Get(Cx, Y - 1, Cz));
			}

			for (unsigned int Cy = Y; Cy < Fy; Cy++)
			{
				Block_ID Block_Type = Voxel_Type(Input->Get(Cx, Cy, Cz));

				Input->Set(Cx, Cy, Cz, Make_Voxel(Block_Type, Skylight, 0));

				if (Block_Type != id_air)
				{
					if (Skylight != 0)
					{
						Skylight -= 1;
					}
				}
			}
		}
	}
}

// Returns true if a block type is not fully opaque.

inline bool Transparent(Block_ID Type)
{
	return (Type == id_air || Type == id_oak_leaves || Type == id_birch_leaves);
}

// Convert a subset of a world into a mesh, and store it in World_Mesh_Target.

unsigned int World_Subset_To_Mesh
(
	World* Input, 

	unsigned int X, 
	unsigned int Y, 
	unsigned int Z, 

	unsigned int X_Res, 
	unsigned int Y_Res, 
	unsigned int Z_Res, 

	float* Target
)
{
	float* P = Target;

	unsigned int Fx = X + X_Res;
	unsigned int Fy = Y + Y_Res;
	unsigned int Fz = Z + Z_Res;

	for (unsigned int Cx = X; Cx < Fx; Cx++)
	{
		for (unsigned int Cy = Y; Cy < Fy; Cy++)
		{
			for (unsigned int Cz = Z; Cz < Fz; Cz++)
			{
				// Skip voxels that are out of bounds.

				if (Input->Out_Of_Bounds(Cx, Cy, Cz))
				{
					continue;
				}

				// Get the block type.

				Block_ID Block_Type = Voxel_Type(Input->Get(Cx, Cy, Cz));

				// Ignore air blocks.

				if (Block_Type == id_air)
				{
					continue;
				}

				// Get the layer indices for each face of the block.

				float Layer_Top = Block_Face_Info[Block_Type]->Top;

				float Layer_Bottom = Block_Face_Info[Block_Type]->Bottom;

				float Layer_Left = Block_Face_Info[Block_Type]->Left;

				float Layer_Right = Block_Face_Info[Block_Type]->Right;

				float Layer_Front = Block_Face_Info[Block_Type]->Front;

				float Layer_Back = Block_Face_Info[Block_Type]->Back;

				// Get the illumination of the block.

				float Block_Lighting = float(Voxel_Skylight(Input->Get(Cx, Cy, Cz))) / 15.0f;

				float Block_Lighting_Left = Block_Lighting * 0.7;

				float Block_Lighting_Front = Block_Lighting * 0.9;

				// Calculate visible faces.

				bool Visible_Top;

				bool Visible_Bottom;

				bool Visible_Left;

				bool Visible_Right;

				bool Visible_Front;

				bool Visible_Back;

				if (Cy == 0)
				{
					Visible_Top = true;
				}
				else
				{
					Block_ID Block_Other = Voxel_Type(Input->Get(Cx, Cy - 1, Cz));

					Visible_Top = Transparent(Block_Other);
				}

				if (Cy == Input->Y_Res - 1)
				{
					Visible_Bottom = true;
				}
				else
				{
					Block_ID Block_Other = Voxel_Type(Input->Get(Cx, Cy + 1, Cz));

					Visible_Bottom = Transparent(Block_Other);
				}

				if (Cx == 0)
				{
					Visible_Left = true;
				}
				else
				{
					Block_ID Block_Other = Voxel_Type(Input->Get(Cx - 1, Cy, Cz));

					Visible_Left = Transparent(Block_Other);
				}

				if (Cx == Input->X_Res - 1)
				{
					Visible_Right = true;
				}
				else
				{
					Block_ID Block_Other = Voxel_Type(Input->Get(Cx + 1, Cy, Cz));

					Visible_Right = Transparent(Block_Other);
				}

				if (Cz == Input->Z_Res - 1)
				{
					Visible_Front = true;
				}
				else
				{
					Block_ID Block_Other = Voxel_Type(Input->Get(Cx, Cy, Cz + 1));

					Visible_Front = Transparent(Block_Other);
				}

				if (Cz == 0)
				{
					Visible_Back = true;
				}
				else
				{
					Block_ID Block_Other = Voxel_Type(Input->Get(Cx, Cy, Cz - 1));

					Visible_Back = Transparent(Block_Other);
				}

				// Render visible faces.

				if (Visible_Top)
				{
					// Top face.

					*(P++) = 1.0f + Cx; *(P++) = 0.0f + Cy; *(P++) = 1.0f + Cz; *(P++) = 1.0f; *(P++) = 0.0f; *(P++) = Layer_Top; *(P++) = Block_Lighting;
					*(P++) = 0.0f + Cx; *(P++) = 0.0f + Cy; *(P++) = 1.0f + Cz; *(P++) = 1.0f; *(P++) = 1.0f; *(P++) = Layer_Top; *(P++) = Block_Lighting;
					*(P++) = 0.0f + Cx; *(P++) = 0.0f + Cy; *(P++) = 0.0f + Cz; *(P++) = 0.0f; *(P++) = 1.0f; *(P++) = Layer_Top; *(P++) = Block_Lighting;

					*(P++) = 1.0f + Cx; *(P++) = 0.0f + Cy; *(P++) = 1.0f + Cz; *(P++) = 1.0f; *(P++) = 0.0f; *(P++) = Layer_Top; *(P++) = Block_Lighting;
					*(P++) = 0.0f + Cx; *(P++) = 0.0f + Cy; *(P++) = 0.0f + Cz; *(P++) = 0.0f; *(P++) = 1.0f; *(P++) = Layer_Top; *(P++) = Block_Lighting;
					*(P++) = 1.0f + Cx; *(P++) = 0.0f + Cy; *(P++) = 0.0f + Cz; *(P++) = 0.0f; *(P++) = 0.0f; *(P++) = Layer_Top; *(P++) = Block_Lighting;
				}

				if (Visible_Bottom)
				{
					// Bottom face.

					*(P++) = 0.0f + Cx; *(P++) = 1.0f + Cy; *(P++) = 0.0f + Cz; *(P++) = 1.0f; *(P++) = 0.0f; *(P++) = Layer_Bottom; *(P++) = Block_Lighting;
					*(P++) = 0.0f + Cx; *(P++) = 1.0f + Cy; *(P++) = 1.0f + Cz; *(P++) = 1.0f; *(P++) = 1.0f; *(P++) = Layer_Bottom; *(P++) = Block_Lighting;
					*(P++) = 1.0f + Cx; *(P++) = 1.0f + Cy; *(P++) = 1.0f + Cz; *(P++) = 0.0f; *(P++) = 1.0f; *(P++) = Layer_Bottom; *(P++) = Block_Lighting;

					*(P++) = 0.0f + Cx; *(P++) = 1.0f + Cy; *(P++) = 0.0f + Cz; *(P++) = 1.0f; *(P++) = 0.0f; *(P++) = Layer_Bottom; *(P++) = Block_Lighting;
					*(P++) = 1.0f + Cx; *(P++) = 1.0f + Cy; *(P++) = 1.0f + Cz; *(P++) = 0.0f; *(P++) = 1.0f; *(P++) = Layer_Bottom; *(P++) = Block_Lighting;
					*(P++) = 1.0f + Cx; *(P++) = 1.0f + Cy; *(P++) = 0.0f + Cz; *(P++) = 0.0f; *(P++) = 0.0f; *(P++) = Layer_Bottom; *(P++) = Block_Lighting;
				}

				if (Visible_Left)
				{
					// Left face.

					*(P++) = 0.0f + Cx; *(P++) = 0.0f + Cy; *(P++) = 1.0f + Cz; *(P++) = 1.0f; *(P++) = 0.0f; *(P++) = Layer_Left; *(P++) = Block_Lighting_Left;
					*(P++) = 0.0f + Cx; *(P++) = 1.0f + Cy; *(P++) = 1.0f + Cz; *(P++) = 1.0f; *(P++) = 1.0f; *(P++) = Layer_Left; *(P++) = Block_Lighting_Left;
					*(P++) = 0.0f + Cx; *(P++) = 1.0f + Cy; *(P++) = 0.0f + Cz; *(P++) = 0.0f; *(P++) = 1.0f; *(P++) = Layer_Left; *(P++) = Block_Lighting_Left;

					*(P++) = 0.0f + Cx; *(P++) = 0.0f + Cy; *(P++) = 1.0f + Cz; *(P++) = 1.0f; *(P++) = 0.0f; *(P++) = Layer_Left; *(P++) = Block_Lighting_Left;
					*(P++) = 0.0f + Cx; *(P++) = 1.0f + Cy; *(P++) = 0.0f + Cz; *(P++) = 0.0f; *(P++) = 1.0f; *(P++) = Layer_Left; *(P++) = Block_Lighting_Left;
					*(P++) = 0.0f + Cx; *(P++) = 0.0f + Cy; *(P++) = 0.0f + Cz; *(P++) = 0.0f; *(P++) = 0.0f; *(P++) = Layer_Left; *(P++) = Block_Lighting_Left;
				}

				if (Visible_Right)
				{
					// Right face.

					*(P++) = 1.0f + Cx; *(P++) = 0.0f + Cy; *(P++) = 0.0f + Cz; *(P++) = 1.0f; *(P++) = 0.0f; *(P++) = Layer_Right; *(P++) = Block_Lighting_Left;
					*(P++) = 1.0f + Cx; *(P++) = 1.0f + Cy; *(P++) = 0.0f + Cz; *(P++) = 1.0f; *(P++) = 1.0f; *(P++) = Layer_Right; *(P++) = Block_Lighting_Left;
					*(P++) = 1.0f + Cx; *(P++) = 1.0f + Cy; *(P++) = 1.0f + Cz; *(P++) = 0.0f; *(P++) = 1.0f; *(P++) = Layer_Right; *(P++) = Block_Lighting_Left;

					*(P++) = 1.0f + Cx; *(P++) = 0.0f + Cy; *(P++) = 0.0f + Cz; *(P++) = 1.0f; *(P++) = 0.0f; *(P++) = Layer_Right; *(P++) = Block_Lighting_Left;
					*(P++) = 1.0f + Cx; *(P++) = 1.0f + Cy; *(P++) = 1.0f + Cz; *(P++) = 0.0f; *(P++) = 1.0f; *(P++) = Layer_Right; *(P++) = Block_Lighting_Left;
					*(P++) = 1.0f + Cx; *(P++) = 0.0f + Cy; *(P++) = 1.0f + Cz; *(P++) = 0.0f; *(P++) = 0.0f; *(P++) = Layer_Right; *(P++) = Block_Lighting_Left;
				}

				if (Visible_Front)
				{
					// Front face.

					*(P++) = 1.0f + Cx; *(P++) = 0.0f + Cy; *(P++) = 1.0f + Cz; *(P++) = 1.0f; *(P++) = 0.0f; *(P++) = Layer_Front; *(P++) = Block_Lighting_Front;
					*(P++) = 1.0f + Cx; *(P++) = 1.0f + Cy; *(P++) = 1.0f + Cz; *(P++) = 1.0f; *(P++) = 1.0f; *(P++) = Layer_Front; *(P++) = Block_Lighting_Front;
					*(P++) = 0.0f + Cx; *(P++) = 1.0f + Cy; *(P++) = 1.0f + Cz; *(P++) = 0.0f; *(P++) = 1.0f; *(P++) = Layer_Front; *(P++) = Block_Lighting_Front;

					*(P++) = 1.0f + Cx; *(P++) = 0.0f + Cy; *(P++) = 1.0f + Cz; *(P++) = 1.0f; *(P++) = 0.0f; *(P++) = Layer_Front; *(P++) = Block_Lighting_Front;
					*(P++) = 0.0f + Cx; *(P++) = 1.0f + Cy; *(P++) = 1.0f + Cz; *(P++) = 0.0f; *(P++) = 1.0f; *(P++) = Layer_Front; *(P++) = Block_Lighting_Front;
					*(P++) = 0.0f + Cx; *(P++) = 0.0f + Cy; *(P++) = 1.0f + Cz; *(P++) = 0.0f; *(P++) = 0.0f; *(P++) = Layer_Front; *(P++) = Block_Lighting_Front;
				}

				if (Visible_Back)
				{
					// Back face.

					*(P++) = 0.0f + Cx; *(P++) = 0.0f + Cy; *(P++) = 0.0f + Cz; *(P++) = 1.0f; *(P++) = 0.0f; *(P++) = Layer_Back; *(P++) = Block_Lighting_Front;
					*(P++) = 0.0f + Cx; *(P++) = 1.0f + Cy; *(P++) = 0.0f + Cz; *(P++) = 1.0f; *(P++) = 1.0f; *(P++) = Layer_Back; *(P++) = Block_Lighting_Front;
					*(P++) = 1.0f + Cx; *(P++) = 1.0f + Cy; *(P++) = 0.0f + Cz; *(P++) = 0.0f; *(P++) = 1.0f; *(P++) = Layer_Back; *(P++) = Block_Lighting_Front;

					*(P++) = 0.0f + Cx; *(P++) = 0.0f + Cy; *(P++) = 0.0f + Cz; *(P++) = 1.0f; *(P++) = 0.0f; *(P++) = Layer_Back; *(P++) = Block_Lighting_Front;
					*(P++) = 1.0f + Cx; *(P++) = 1.0f + Cy; *(P++) = 0.0f + Cz; *(P++) = 0.0f; *(P++) = 1.0f; *(P++) = Layer_Back; *(P++) = Block_Lighting_Front;
					*(P++) = 1.0f + Cx; *(P++) = 0.0f + Cy; *(P++) = 0.0f + Cz; *(P++) = 0.0f; *(P++) = 0.0f; *(P++) = Layer_Back; *(P++) = Block_Lighting_Front;
				}
			}	
		}	
	}

	// Return the mesh size.

	return P - Target;
}