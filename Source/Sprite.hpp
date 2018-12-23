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

// Write a vertex to a managed sprite.

inline void Write_Vertex(Managed_Sprite* The_Managed_Sprite, float X, float Y, float Xt, float Yt)
{
	The_Managed_Sprite->Vertices[The_Managed_Sprite->Offset++] = X;
	The_Managed_Sprite->Vertices[The_Managed_Sprite->Offset++] = Y;

	The_Managed_Sprite->Vertices[The_Managed_Sprite->Offset++] = Xt;
	The_Managed_Sprite->Vertices[The_Managed_Sprite->Offset++] = Yt;
}

// A structure that represents a sprite loaded to the GPU.

struct Final_Sprite
{
	GLuint Sprite_VAO;
	GLuint Sprite_VBO;

	GLuint Sprite_Texture;

	unsigned int Size_In_Vertices;
};

// Generate a Final_Sprite* using a Managed_Sprite*.

Final_Sprite* Make_Final_Sprite(Managed_Sprite* The_Managed_Sprite, GLuint The_Texture)
{
	Final_Sprite* The_Final_Sprite = new Final_Sprite();

	The_Final_Sprite->Sprite_Texture = The_Texture;

	The_Final_Sprite->Size_In_Vertices = The_Managed_Sprite->Size_In_Vertices;

	// Do all the OpenGL stuff that's needed to create a texture.

	glGenVertexArrays(1, &The_Final_Sprite->Sprite_VAO);

	glGenBuffers(1, &The_Final_Sprite->Sprite_VBO);

	glBindVertexArray(The_Final_Sprite->Sprite_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, The_Final_Sprite->Sprite_VBO);

	glBufferData(GL_ARRAY_BUFFER, The_Managed_Sprite->Size_In_Bytes, The_Managed_Sprite->Vertices, GL_STATIC_DRAW);

	// Coordinate pointer.

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(float)));

	glEnableVertexAttribArray(0);

	// Texture coordinate pointer.

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glEnableVertexAttribArray(1);

	// Completely obliterate the Managed_Sprite*.

	free(The_Managed_Sprite->Vertices);

	delete The_Managed_Sprite;

	// The final sprite is finalized, we can now return it. Don't forget to delete it after you
	// are done with it!

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return The_Final_Sprite;
}

// Draw a Final_Sprite*.

void Draw_Sprite(Final_Sprite* The_Final_Sprite)
{
	// Enable alpha blending.

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable the sprite shader program.

	glUseProgram(Sprite_Program);

	// Bind the vertex array object to the current state.

	glBindVertexArray(The_Final_Sprite->Sprite_VAO);

	// Enable the final sprite's texture.

	glBindTexture(GL_TEXTURE_2D, The_Final_Sprite->Sprite_Texture);

	// Draw the vertex array object as an array of triangles.

	glDrawArrays(GL_TRIANGLES, 0, The_Final_Sprite->Size_In_Vertices);

	// Disable the final sprite's texture.

	glBindTexture(GL_TEXTURE_2D, 0);

	// Unbind the vertex array object from the current state.

	glBindVertexArray(0);

	// Disable the sprite shader program.

	glUseProgram(0);

	// Destroy the sprite (wait, maybe we will want to reuse a sprite, but whatever).

	glDeleteVertexArrays(1, &The_Final_Sprite->Sprite_VAO);

	glDeleteBuffers(1, &The_Final_Sprite->Sprite_VBO);

	// Disable alpha blending.

	glDisable(GL_BLEND);
}

// Generate a test sprite (this looks terrible).

Final_Sprite* Generate_Test_Sprite()
{
	Managed_Sprite* My_Managed_Sprite = Allocate_Sprite(6);

	Write_Vertex(My_Managed_Sprite, -1.0f, +1.0f, 0.0f, 0.0f);
	Write_Vertex(My_Managed_Sprite, -1.0f, -1.0f, 0.0f, 1.0f);
	Write_Vertex(My_Managed_Sprite, +1.0f, -1.0f, 1.0f, 1.0f);

	Write_Vertex(My_Managed_Sprite, -1.0f, +1.0f, 0.0f, 0.0f);
	Write_Vertex(My_Managed_Sprite, +1.0f, -1.0f, 1.0f, 1.0f);
	Write_Vertex(My_Managed_Sprite, +1.0f, +1.0f, 1.0f, 0.0f);

	Final_Sprite* My_Final_Sprite = Make_Final_Sprite(My_Managed_Sprite, Interface_Textures["logo"]->Storage);

	return My_Final_Sprite;
}

// Generate a rectangle sprite (in screen coordinates).

Final_Sprite* Generate_Rectangle_Sprite(int X_tl, int Y_tl, int X_br, int Y_br, int X_Res, int Y_Res, GLuint Texture)
{
	Managed_Sprite* My_Managed_Sprite = Allocate_Sprite(6);

	float X_tlf = X_tl / float(X_Res) * 2.0f - 1.0f;
	float Y_tlf = Y_tl / float(Y_Res) * 2.0f - 1.0f;

	float X_brf = X_br / float(X_Res) * 2.0f - 1.0f;
	float Y_brf = Y_br / float(Y_Res) * 2.0f - 1.0f;

	Write_Vertex(My_Managed_Sprite, X_tlf, -Y_tlf, 0.0f, 0.0f);
	Write_Vertex(My_Managed_Sprite, X_tlf, -Y_brf, 0.0f, 1.0f);
	Write_Vertex(My_Managed_Sprite, X_brf, -Y_brf, 1.0f, 1.0f);

	Write_Vertex(My_Managed_Sprite, X_tlf, -Y_tlf, 0.0f, 0.0f);
	Write_Vertex(My_Managed_Sprite, X_brf, -Y_brf, 1.0f, 1.0f);
	Write_Vertex(My_Managed_Sprite, X_brf, -Y_tlf, 1.0f, 0.0f);

	Final_Sprite* My_Final_Sprite = Make_Final_Sprite(My_Managed_Sprite, Texture);

	return My_Final_Sprite;
}