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

