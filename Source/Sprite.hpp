#include <iostream>

// You must call Generate_Sprite_Preliminaries() before doing any sprite based operations.

GLuint Sprite_Program;

void Generate_Sprite_Preliminaries()
{
	Sprite_Program = Load_Program("../Shaders/Sprite_Vertex.glsl", "../Shaders/Sprite_Fragment.glsl");
}

// A structure that represents a managed sprite.

struct Managed_Sprite
{
	// Stuff that concerns the memory of the sprite.

	float* Vertices;

	unsigned int Size_In_Bytes;

	unsigned int Size_In_Floats;

	unsigned int Size_In_Vertices;

	// Stuff that concerns any stream writers to this sprite.

	unsigned int Offset;
};

// Allocate a chunk of memory (on the CPU) that can hold Vertex_Count 2-dimensional vertices with
// 2-dimensional texture coordinates.

Managed_Sprite* Allocate_Sprite(unsigned int Vertex_Count)
{
	float* Memory = (float*)malloc(Vertex_Count * 4 * sizeof(float));

	if (!Memory)
	{
		std::cout << "Could not allocate a sprite buffer for " << Vertex_Count << " vertices." << std::endl;

		exit(16);
	}

	Managed_Sprite* The_Managed_Sprite = new Managed_Sprite();

	The_Managed_Sprite->Vertices = Memory;

	The_Managed_Sprite->Size_In_Bytes = Vertex_Count * 4 * sizeof(float);

	The_Managed_Sprite->Size_In_Floats = Vertex_Count * 4;

	The_Managed_Sprite->Size_In_Vertices = Vertex_Count;

	The_Managed_Sprite->Offset = 0;

	return The_Managed_Sprite;
}

