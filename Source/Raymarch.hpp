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

// I somehow managed to make this function work. I'm honestly so confused as to how I managed to
// do this without a tutorial. But it works, so don't touch it.

// It will do a raytrace from a given point along a given direction and find out if the ray 
// managed to hit something within a given range. It will also tell you what it hit, and what side
// of the object it hit.

Raymarch_Data Raymarch(World* The_World, float Ox, float Oy, float Oz, float Look_X, float Look_Y, float Breakout = 32.0f)
{
	Raymarch_Data Out;

	float Px = Ox;
	float Py = Oy;
	float Pz = Oz;

	float Ix = -sin(Radians(Look_Y));
	float Iy = -tan(Radians(Look_X));
	float Iz = -cos(Radians(Look_Y));

	float Total_Distance = 0.0f;

	unsigned int Total_Steps = 0;

	while (true)
	{
		// Check for collisions.

		if (Voxel_Type(The_World->Get_Safe(int(Px), int(Py), int(Pz))) != id_air)
		{
			float X_n = std::floor(Px);
			float Y_n = std::floor(Py);
			float Z_n = std::floor(Pz);

			float X_p = std::ceil(Px);
			float Y_p = std::ceil(Py);
			float Z_p = std::ceil(Pz);

			float X_c = std::floor(Px) + 0.5f;
			float Y_c = std::floor(Py) + 0.5f;
			float Z_c = std::floor(Pz) + 0.5f;

			float D_nx = (Px - X_n) * (Px - X_n) + (Py - Y_c) * (Py - Y_c) + (Pz - Z_c) * (Pz - Z_c);
			float D_px = (Px - X_p) * (Px - X_p) + (Py - Y_c) * (Py - Y_c) + (Pz - Z_c) * (Pz - Z_c);

			float D_ny = (Px - X_c) * (Px - X_c) + (Py - Y_n) * (Py - Y_n) + (Pz - Z_c) * (Pz - Z_c);
			float D_py = (Px - X_c) * (Px - X_c) + (Py - Y_p) * (Py - Y_p) + (Pz - Z_c) * (Pz - Z_c);

			float D_nz = (Px - X_c) * (Px - X_c) + (Py - Y_c) * (Py - Y_c) + (Pz - Z_n) * (Pz - Z_n);
			float D_pz = (Px - X_c) * (Px - X_c) + (Py - Y_c) * (Py - Y_c) + (Pz - Z_p) * (Pz - Z_p);

			float D_all[6] = {D_nx, D_px, D_ny, D_py, D_nz, D_pz};

			unsigned int Low_idx = 0;

			float Low_val = D_all[Low_idx];

			for (int i = 0; i < 6; i++)
			{
				if (D_all[i] < Low_val)
				{
					Low_idx = i;

					Low_val = D_all[Low_idx];
				}
			}

			Out.Hit = true;

			Out.Hit_X = Px;
			Out.Hit_Y = Py;
			Out.Hit_Z = Pz;

			Out.Hit_Side = Low_idx;

			break;
		}

		// Increment ray position.

		float Distance = 0.01f;

		float X_n = std::floor(Px);
		float Y_n = std::floor(Py);
		float Z_n = std::floor(Pz);

		float X_p = std::ceil(Px);
		float Y_p = std::ceil(Py);
		float Z_p = std::ceil(Pz);

		Px += Ix * Distance;
		Py += Iy * Distance;
		Pz += Iz * Distance;

		Total_Steps++;

		if (Total_Distance > Breakout || Total_Steps > Breakout * 128)
		{
			Out.Hit = false;

			break;
		}
	}

	return Out;
}