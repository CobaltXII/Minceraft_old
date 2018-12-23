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
