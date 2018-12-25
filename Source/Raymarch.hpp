// A structure to hold the information after a raymarch.

struct Raymarch_Data
{
	// Did an intersection even happen?

	bool Hit;

	// Voxel of intersection coordinates.

	unsigned int Hit_X;
	unsigned int Hit_Y;
	unsigned int Hit_Z;

	// The side of the intersection.

	unsigned int Hit_Side;
};
