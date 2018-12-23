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

// Create a chunk.

Chunk* Make_Chunk
(
	World* Input, 

	unsigned int X, 
	unsigned int Y, 
	unsigned int Z, 

	unsigned int X_Res, 
	unsigned int Y_Res, 
	unsigned int Z_Res
)
{
	// Allocate the absolute maximum amount of memory required to represent a subset of the given
	// size of any given world. Most of this will be unused.

	float* Target = (float*)malloc(X_Res * Y_Res * Z_Res * 6 * 2 * 3 * 7 * sizeof(float));

	// Generate the mesh that represents the given subset.

	unsigned int Target_Size_In_Floats = World_Subset_To_Mesh(Input, X, Y, Z, X_Res, Y_Res, Z_Res, Target);

	// Create the chunk.

	Chunk* The_Chunk = new Chunk(Target_Size_In_Floats, X, Y, Z, X_Res, Y_Res, Z_Res);

	// Use OpenGL to generate a VBO and a VAO.

	GLuint Target_VBO;
	GLuint Target_VAO;

	glGenVertexArrays(1, &Target_VAO);

	glGenBuffers(1, &Target_VBO);

	// Bind the VBO and the VAO to the current state.

	glBindVertexArray(Target_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, Target_VBO);

	// Upload the target data to the GPU.

	glBufferData(GL_ARRAY_BUFFER, Target_Size_In_Floats * sizeof(float), Target, GL_STATIC_DRAW);

	// Free the data from the CPU, as it's on the GPU now.

	free(Target);

	// Enable the default vertex attribute pointers.

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(0 * sizeof(float)));

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));

	glEnableVertexAttribArray(2);

	// Unbind the VBO and the VAO from the current state.

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Remember the VAO identifier and return the chunk.

	The_Chunk->Vertex_Array = Target_VAO;

	The_Chunk->Buffer_Object = Target_VBO;

	return The_Chunk;
}
