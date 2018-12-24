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
