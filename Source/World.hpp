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

