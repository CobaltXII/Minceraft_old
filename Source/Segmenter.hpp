// A segmenter makes it easy to handle chunks and worlds together as one object.

struct Segmenter
{
	World* The_World;

	unsigned int Chunk_X_Res;
	unsigned int Chunk_Y_Res;
	unsigned int Chunk_Z_Res;

	unsigned int Chunk_Count;

	Chunk** The_Chunks;

