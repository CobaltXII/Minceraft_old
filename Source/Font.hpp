// You must call Generate_Font_Preliminaries() before doing any font based operations.

GLuint Font_Program;

void Generate_Font_Preliminaries()
{
	Font_Program = Load_Program("../Shaders/Font_Vertex.glsl", "../Shaders/Font_Fragment.glsl");
}

// Font color enumerations.

enum Font_Color
{
	black,

	dark_blue,

	dark_green,

	dark_aqua,

	dark_red,

	dark_purple,

	gold,

	gray,

	dark_gray,

	blue,

	green,

	aqua,

	red,

	light_purple,

	yellow,

	white
};

// Render a string of text.

void Render_Text(std::string String, int X, int Y, int X_Res, int Y_Res, int Factor, Font_Color Color_Index = white, bool Darkened = false)
{
	// String vertices have a position, a texture coordinate, and a color index. That gives us
	// 5 floats for a vertex. Each character is composed of 6 vertices, so the amount of memory
	// required is equivalent to 5 * 6 * String.size() * sizeof(float).

	float* Vertices = (float*)malloc(5 * 6 * String.size() * sizeof(float));

	float* P = Vertices;

	// Hey, we need to update this so that font glyphs have variables size.

	float Font_Size = 8.0f * Factor;

	float Font_Size_With_Spacing = 9.0f * Factor;

	for (int i = 0; i < String.size(); i++)
	{
		// Screen coordinates.

		float X_l = X + i * Font_Size_With_Spacing; 

		float X_r = X + i * Font_Size_With_Spacing + Font_Size;

		float Y_t = Y;

		float Y_b = Y + Font_Size;

		X_l = X_l / float(X_Res) * 2.0f - 1.0f;

		X_r = X_r / float(X_Res) * 2.0f - 1.0f;

		Y_t = -(Y_t / float(Y_Res) * 2.0f - 1.0f);

		Y_b = -(Y_b / float(Y_Res) * 2.0f - 1.0f);

		// Texture coordinates.

		unsigned char Ascii = String[i];

		unsigned char Ascii_X = Ascii % 16;

		unsigned char Ascii_Y = Ascii / 16;

		float Xt_l = float(Ascii_X) * 8.0f / 128.0f;

		float Xt_r = (float(Ascii_X) * 8.0f + 8.0f) / 128.0f;

		float Yt_t = (float(Ascii_Y) * 8.0f / 128.0f);

		float Yt_b = (float(Ascii_Y) * 8.0f + 8.0f) / 128.0f;

		// Top left.

		*(P++) = X_l;
		*(P++) = Y_t;

		*(P++) = Xt_l;
		*(P++) = Yt_t;

		*(P++) = float(Color_Index) + 16 * Darkened;

		// Top right.

		*(P++) = X_r;
		*(P++) = Y_t;

		*(P++) = Xt_r;
		*(P++) = Yt_t;

		*(P++) = float(Color_Index) + 16 * Darkened;

		// Bottom left.

		for (int j = 0; j < 2; j++)
		{
			*(P++) = X_l;
			*(P++) = Y_b;

			*(P++) = Xt_l;
			*(P++) = Yt_b;

			*(P++) = float(Color_Index) + 16 * Darkened;
		}

		// Top right.

		*(P++) = X_r;
		*(P++) = Y_t;

		*(P++) = Xt_r;
		*(P++) = Yt_t;

		*(P++) = float(Color_Index) + 16 * Darkened;

		// Bottom right.

		*(P++) = X_r;
		*(P++) = Y_b;

		*(P++) = Xt_r;
		*(P++) = Yt_b;

		*(P++) = float(Color_Index) + 16 * Darkened;
	}

	// Generate VAO and VBO.

	GLuint Font_VAO;
	GLuint Font_VBO;

	glGenVertexArrays(1, &Font_VAO);

	glGenBuffers(1, &Font_VBO);

	glBindVertexArray(Font_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, Font_VBO);

	glBufferData(GL_ARRAY_BUFFER, 5 * 6 * String.size() * sizeof(float), Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));

	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Render VAO and VBO.

	glUseProgram(Font_Program);

	glBindVertexArray(Font_VAO);

	glBindTexture(GL_TEXTURE_2D, Interface_Textures["font"]->Storage);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLES, 0, String.size() * 6);

	glDisable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	glBindVertexArray(0);

	glUseProgram(0);

	// Delete the VAO and VBO.

	glDeleteVertexArrays(1, &Font_VAO);

	glDeleteBuffers(1, &Font_VBO);

	free(Vertices);
}