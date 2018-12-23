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

