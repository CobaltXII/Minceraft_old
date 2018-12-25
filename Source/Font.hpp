// You must call Generate_Font_Preliminaries() before doing any font based operations. Also, you
// must call this after Generate_Interface_Textures() is called.

GLuint Font_Program;

struct Glyph_Info
{
	unsigned int Width;

	unsigned int X_l;
	unsigned int X_r;

	Glyph_Info(unsigned int _Width, unsigned int _X_l, unsigned int _X_r)
	{
		Width = _Width;

		X_l = _X_l;
		X_r = _X_r;
	}
};

Glyph_Info** Font_Info;

void Generate_Font_Preliminaries()
{
	Font_Program = Load_Program("../Shaders/Font_Vertex.glsl", "../Shaders/Font_Fragment.glsl");

	// Retrieve the Texture* associated with the font.

	Texture* The_Font = Interface_Textures["font"];

	// Calculate information about the glyphs in the font.

	Font_Info = (Glyph_Info**)malloc(256 * sizeof(Glyph_Info*));

	for (int i = 0; i < 256; i++)
	{
		// I'm not joking around, or just putting this here because it's a common programmer meme.
		// I honestly, truthfully, have no idea why I need to switch X_l and X_r around, because
		// if I don't, the whole thing breaks. Seriously.

		unsigned int X_l = 999;
		unsigned int X_r = 999;

		// Get the texture coordinates corresponding to the glyph.

		int Tx = i % 16 * 8;

		int Ty = i / 16 * 8;

		// Find X_l.

		for (int x = 0; x < 8; x++)
		{
			int Tmx = Tx + x;

			bool Solid = false;

			for (int y = 0; y < 8; y++)
			{
				int Tmy = Ty + y;

				// If the channel count is not 4 (it will be 4 unless someone changes the image),
				// then this will probably either cause a segmentation fault or just return stupid
				// values.

				unsigned char* Offset = The_Font->Data + (Tmx + The_Font->X_Res * Tmy) * 4;

				if (Offset[3] == 255)
				{
					// Found an empty pixel.

					Solid = true;

					break;
				}
			}

			if (Solid)
			{
				// Found a solid vertical stripe of the glyph, we now know the first solid 
				// stripe location.

				X_r = x;
			}
		}

		if (X_r == 999)
		{
			// Damn, it's an empty one. We'll just pretend it doesn't exist...

			Font_Info[i] = nullptr;

			continue;
		}

		// Find X_r.

		for (int x = 0; x < 8; x++)
		{
			int Tmx = Tx + (7 - x);

			bool Solid = false;

			for (int y = 0; y < 8; y++)
			{
				int Tmy = Ty + y;

				// If the channel count is not 4 (it will be 4 unless someone changes the image),
				// then this will probably either cause a segmentation fault or just return stupid
				// values.

				unsigned char* Offset = The_Font->Data + (Tmx + The_Font->X_Res * Tmy) * 4;

				if (Offset[3] == 255)
				{
					// Found an empty pixel.

					Solid = true;

					break;
				}
			}

			if (Solid)
			{
				// Found a solid vertical stripe of the glyph, we now know the first solid 
				// stripe location.

				X_l = 7 - x;
			}
		}

		// We have X_l and X_r (hopefully), so we can easily calculate the width and store it in a
		// structure.

		Font_Info[i] = new Glyph_Info(X_r + 1 - X_l, X_l, X_r + 1);
	}
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

	float Left_X = X;

	for (int i = 0; i < String.size(); i++)
	{
		// Get the character code.

		unsigned char Ascii = String[i];

		if (Font_Info[Ascii] == nullptr)
		{
			Left_X += 8;

			continue;
		}

		// Screen coordinates.

		float X_l = Left_X; 

		float X_r = Left_X + Font_Info[Ascii]->Width * Factor;

		float Y_t = Y;

		float Y_b = Y + 8.0f * Factor;

		X_l = X_l / float(X_Res) * 2.0f - 1.0f;

		X_r = X_r / float(X_Res) * 2.0f - 1.0f;

		Y_t = -(Y_t / float(Y_Res) * 2.0f - 1.0f);

		Y_b = -(Y_b / float(Y_Res) * 2.0f - 1.0f);

		// Texture coordinates.

		unsigned char Ascii_X = Ascii % 16;

		unsigned char Ascii_Y = Ascii / 16;

		float Xt_l = (float(Ascii_X) * 8.0f + Font_Info[Ascii]->X_l) / 128.0f;

		float Xt_r = (float(Ascii_X) * 8.0f + Font_Info[Ascii]->X_r) / 128.0f;

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

		// Increment position.

		Left_X += (Font_Info[Ascii]->Width + 1) * Factor;
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

// Render a string of text with a shadow.

inline void Render_Shadowed_Text(std::string String, int X, int Y, int X_Res, int Y_Res, int Factor, Font_Color Color_Index = white)
{
	Render_Text(String, X + Factor, Y + Factor, X_Res, Y_Res, Factor, Color_Index, true);

	Render_Text(String, X, Y, X_Res, Y_Res, Factor, Color_Index, false);
}

// Render a string of text that is centered horizontally.

inline void Render_Centered_Text(std::string String, int X, int Y, int X_Res, int Y_Res, int Factor, Font_Color Color_Index = white)
{
	unsigned int String_Width = 0;

	for (int i = 0; i < String.size(); i++)
	{
		if (Font_Info[(unsigned char)(String[i])] == nullptr)
		{
			String_Width += 8;

			continue;
		}

		String_Width += Font_Info[(unsigned char)(String[i])]->Width + 1;
	}

	Render_Text(String, X - String_Width / 2 * Factor, Y, X_Res, Y_Res, Factor, Color_Index, false);
}

// Render a string of text that is centered horizontally with a shadow.
}