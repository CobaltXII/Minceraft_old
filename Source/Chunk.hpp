#include <ostream>

// A chunk represents a subset of a world. The chunk holds information about the mesh associated
// with that subset of the world.

struct Chunk
{
	unsigned int X;
	unsigned int Y;
	unsigned int Z;

	unsigned int X_Res; 
	unsigned int Y_Res; 
	unsigned int Z_Res;

	GLuint Vertex_Array;

	GLuint Buffer_Object;

	unsigned int Vertex_Array_Size_In_Floats;

	// The boolean value of the variable Live indicates whether this chunk is loaded onto the GPU
	// or not. Chunks that are not loaded onto the GPU are not rendered.

	bool Live;

	// The boolean value of the variable Modified indicates whether this chunk's blocks have been
	// modified since the last meshification. If true, this chunk is regenerated.

	bool Modified;
