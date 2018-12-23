// A segmenter makes it easy to handle chunks and worlds together as one object.

struct Segmenter
{
	World* The_World;

	unsigned int Chunk_X_Res;
	unsigned int Chunk_Y_Res;
	unsigned int Chunk_Z_Res;

	unsigned int Chunk_Count;

	Chunk** The_Chunks;

	// Safely set a block in the world. This function will update the chunks of this segmenter.
	// The chunk updates are gathered together and updated at the end of a frame, so that 
	// individual chunks are updated only once.

	inline void Set_Safe(unsigned int X, unsigned int Y, unsigned int Z, Voxel Value)
	{
		if (X < 0 || Y < 0 || Z < 0 || X > The_World->X_Res - 1 || Y > The_World->Y_Res - 1 || Z > The_World->Z_Res - 1)
		{
			// Out of bounds.

			return;
		}

		Voxel Old_Value = The_World->Voxels[X + The_World->X_Res * (Y + The_World->Y_Res * Z)];

		if (Old_Value != Value)
		{
			// Something actually changed.

			The_World->Voxels[X + The_World->X_Res * (Y + The_World->Y_Res * Z)] = Value;

			The_Chunks[(X / 16) + Chunk_X_Res * ((Y / 16) + Chunk_Y_Res * (Z / 16))]->Modified = true;

			// If the new block type is transparent, neighboring chunks may need to be updated.

			if (Transparent(Voxel_Type(Value)))
			{
				unsigned int Xc = X % 16;
				unsigned int Yc = Y % 16;
				unsigned int Zc = Z % 16;

				// Negative X.

				if (X != 0 && Xc == 0)
				{
					The_Chunks[(X / 16 - 1) + Chunk_X_Res * ((Y / 16) + Chunk_Y_Res * (Z / 16))]->Modified = true;
				}

				// Positive X.

				if (X != The_World->X_Res - 1 && Xc == 15)
				{
					The_Chunks[(X / 16 + 1) + Chunk_X_Res * ((Y / 16) + Chunk_Y_Res * (Z / 16))]->Modified = true;
				}

				// Negative Y.

				if (Y != 0 && Yc == 0)
				{
					The_Chunks[(X / 16) + Chunk_X_Res * ((Y / 16 - 1) + Chunk_Y_Res * (Z / 16))]->Modified = true;
				}

				// Positive Y.

				if (Y != The_World->Y_Res - 1 && Yc == 15)
				{
					The_Chunks[(X / 16) + Chunk_X_Res * ((Y / 16 + 1) + Chunk_Y_Res * (Z / 16))]->Modified = true;
				}

				// Negative Z.

				if (Z != 0 && Zc == 0)
				{
					The_Chunks[(X / 16) + Chunk_X_Res * ((Y / 16) + Chunk_Y_Res * (Z / 16 - 1))]->Modified = true;
				}

				// Positive Z.

				if (Z != The_World->Z_Res - 1 && Zc == 15)
				{
					The_Chunks[(X / 16) + Chunk_X_Res * ((Y / 16) + Chunk_Y_Res * (Z / 16 + 1))]->Modified = true;
				}
			}

			// Update chunks below the block being set's position if sky lighting will change.

			if ((Voxel_Type(Value) == id_air) != (Voxel_Type(Old_Value) == id_air))
			{
				for (unsigned int Py = Y / 16; Py < Chunk_Y_Res; Py++)
				{
					The_Chunks[(X / 16) + Chunk_X_Res * (Py + Chunk_Y_Res * (Z / 16))]->Modified = true;
				}
			}
		}
	}

	// Same as above, but sets the light values of the block being set to the light values of the
	// current block at the same position as the block being set.

	inline void Set_Safe_Unlit(unsigned int X, unsigned int Y, unsigned int Z, Block_ID Value)
	{
		Voxel Current_Value = The_World->Get_Safe(X, Y, Z);

		Set_Safe(X, Y, Z, Make_Voxel(Value, Voxel_Skylight(Current_Value), Voxel_Light(Current_Value)));
	}
};

// Create a segmenter from a world.

Segmenter* Make_Segmenter(World* A_World)
{
	Segmenter* The_Segmenter = new Segmenter();

	The_Segmenter->The_World = A_World;

	The_Segmenter->Chunk_X_Res = A_World->X_Res / 16;
	The_Segmenter->Chunk_Y_Res = A_World->Y_Res / 16;
	The_Segmenter->Chunk_Z_Res = A_World->Z_Res / 16;

	The_Segmenter->Chunk_Count = The_Segmenter->Chunk_X_Res * The_Segmenter->Chunk_Y_Res * The_Segmenter->Chunk_Z_Res;

	The_Segmenter->The_Chunks = (Chunk**)malloc(The_Segmenter->Chunk_Count * sizeof(Chunk*));

	// Generate the chunks. Currently, every chunk is loaded to the GPU.

	for (int X = 0; X < The_Segmenter->Chunk_X_Res; X++)
	{
		for (int Y = 0; Y < The_Segmenter->Chunk_Y_Res; Y++)
		{
			for (int Z = 0; Z < The_Segmenter->Chunk_Z_Res; Z++)
			{
				The_Segmenter->The_Chunks[X + The_Segmenter->Chunk_X_Res * (Y + The_Segmenter->Chunk_Y_Res * Z)] = Make_Chunk(A_World, X * 16, Y * 16, Z * 16, 16, 16, 16);
			}
		}
	}

	// Done, return the segmenter.

	return The_Segmenter;
}