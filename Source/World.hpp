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

