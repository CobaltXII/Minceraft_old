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

	Chunk
	(
		unsigned int _Vertex_Array_Size_In_Floats,

		unsigned int _X, 
		unsigned int _Y, 
		unsigned int _Z, 

		unsigned int _X_Res, 
		unsigned int _Y_Res, 
		unsigned int _Z_Res
	)
	{
		Live = true;

		Modified = false;

		X = _X;
		Y = _Y;
		Z = _Z;

		X_Res = _X_Res;
		Y_Res = _Y_Res;
		Z_Res = _Z_Res;
		
		Vertex_Array = 0;

		Vertex_Array_Size_In_Floats = _Vertex_Array_Size_In_Floats;
	}

	// Destroy this chunk.

	void Destroy()
	{
		Live = false;

		glDeleteVertexArrays(1, &Vertex_Array);

		glDeleteBuffers(1, &Buffer_Object);

		Vertex_Array_Size_In_Floats = 0;
	}
};
