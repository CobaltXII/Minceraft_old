#include <SDL2/SDL.h>

#define GL_SILENCE_DEPRECATION

#include <OpenGL/GL3.h>

#define STB_IMAGE_IMPLEMENTATION

#include "../Libraries/stb_image.hpp"

#include "Minceraft.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

#include <cmath>

// Format a size in bytes.

std::string Byte_Format(unsigned int Bytes)
{
	std::stringstream Format;

	if (Bytes > 1024 * 1024 * 1024)
	{
		Format << float(Bytes) / float(1024 * 1024 * 1024) << " GB";
	}
	else if (Bytes > 1024 * 1024)
	{
		Format << float(Bytes) / float(1024 * 1024) << " MB";
	}
	else if (Bytes > 1024)
	{
		Format << float(Bytes) / float(1024) << " kB";
	}
	else
	{
		Format << float(Bytes) << " B";
	}

	return Format.str();
}

// Current Minceraft version.

std::string Minceraft_Version = "0.0.59";

// Generate the version string for this build of Minceraft.

std::string Make_Minceraft_Version_String()
{
	std::stringstream String_Stream;

	String_Stream << "Minceraft " << Minceraft_Version;

	return String_Stream.str();
}

// Requested dimensions.

int Main_X_Resolution;
int Main_Y_Resolution;

// Actual dimensions.

int Context_X_Resolution;
int Context_Y_Resolution;

// Aspect ratio.

float Aspect_Ratio = 1.0;

// Mouse position.

int Main_Mouse_X = 0;
int Main_Mouse_Y = 0;

// Mouse state.

bool Main_Mouse_L = false;
bool Main_Mouse_R = false;

bool Main_Mouse_Relative = false;

// Iteration counter.

unsigned long Main_Iteration = 0;

// Entry point.

int main(int argc, char** argv)
{
	// Initialize dimensions.

	Main_X_Resolution = 950;
	Main_Y_Resolution = 540;

	// Initialize SDL.

	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		std::cout << "Could not initialize SDL." << std::endl;

		exit(1);
	}

	// Create the SDL_Window.

	SDL_Window* Main_Window = SDL_CreateWindow
	(
		Make_Minceraft_Version_String().c_str(),

		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,

		Main_X_Resolution,
		Main_Y_Resolution,

		SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL
	);

	// Check if the SDL_Window was created successfully.

	if (!Main_Window)
	{
		std::cout << "Could not create the SDL_Window." << std::endl;

		exit(2);
	}

	// Request OpenGL 3.2.

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Request double-buffering.

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create the SDL_GLContext.

	SDL_GLContext Main_Context = SDL_GL_CreateContext(Main_Window);

	if (!Main_Context)
	{
		std::cout << "Could not create the SDL_GLContext." << std::endl;

		exit(3);
	}

	// Set the swap interval to update immediately.

	SDL_GL_SetSwapInterval(0);

	// Set up preliminaries for depth testing.

	glEnable(GL_DEPTH_CLAMP);

	// Set up preliminaries for backface culling.

	glCullFace(GL_BACK);

	glFrontFace(GL_CW);

	// Enable alpha blending.

	if (false)
	{
		// Actually, we don't need alpha blending right now because textures currently only have
		// completely opaque or completely transparent texels. Completely transparent texels are
		// discarded by the fragment shader.

		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	// Get the dimensions and aspect ratio of the OpenGL context.

	SDL_GL_GetDrawableSize(Main_Window, &Context_X_Resolution, &Context_Y_Resolution);

	Aspect_Ratio = float(Context_X_Resolution) / float(Context_Y_Resolution);

	// Load the texture array that contains every block texture.

	GLuint Block_Texture_Array = Generate_Block_Texture_Array();

	Make_Block_Face_Info();

	// Load the interface textures.

	Generate_Interface_Textures();

	// Generate sprite and isometric block preliminaries. Basically loads the sprite and isometric shaders.

	Generate_Sprite_Preliminaries();

	Generate_Isometric_Preliminaries();

	// Load the block shader program.

	GLuint Block_Program = Load_Program("../Shaders/Block_Vertex.glsl", "../Shaders/Block_Fragment.glsl");

	// Load the crosshair shader program.

	GLuint Crosshair_Program = Load_Program("../Shaders/Crosshair_Vertex.glsl", "../Shaders/Crosshair_Fragment.glsl");

	// Load the frame buffer shader program.

	GLuint Frame_Buffer_Program = Load_Program("../Shaders/Frame_Buffer_Vertex.glsl", "../Shaders/Frame_Buffer_Fragment.glsl");

	// Generate the frame buffer.

	Frame_Buffer* The_Frame_Buffer = Build_Frame_Buffer(Context_X_Resolution, Context_Y_Resolution);

	// Generate the projection matrix.

	mat4 Matrix_Projection = Mat4_Empty();

	Mat4_Projection(Matrix_Projection, Radians(70.0f), Aspect_Ratio, 1.0f, 1000.0f);

	// Generate the view matrices.

	mat4 Matrix_View_X = Mat4_Empty();
	mat4 Matrix_View_Y = Mat4_Empty();

	Mat4_Rotate_X(Matrix_View_X, 0.0f);
	Mat4_Rotate_Y(Matrix_View_Y, 0.0f);

	// Generate a fresh world.

	int World_Size = Id_Medium_World;

	World* The_World = Allocate_Empty_World(World_X_Res_List[World_Size], World_Y_Res_List[World_Size], World_Z_Res_List[World_Size]);

	Generate_World(The_World, time(NULL));

	// Generate a segmenter to represent the world.

	Segmenter* The_Segmenter = Make_Segmenter(The_World);

	// Show the window.

	SDL_ShowWindow(Main_Window);

	// Create variables to hold the player's position.

	float Player_X = -float(The_World->X_Res) / 2.0f;

	float Player_Y = 0.0f;

	float Player_Z = -float(The_World->Z_Res) / 2.0f;

	// Create variables to hold the player's velocity.

	float Player_Vx = 0.0f;
	float Player_Vy = 0.0f;
	float Player_Vz = 0.0f;

	// Create variables to hold the player's direction.

	float Look_X = 0.0f;
	float Look_Y = 0.0f;

	// Create a variable to hold the player's selected item.

	int Player_Selection = 0;

	// Enter the main loop.

	bool Main_Running = true;

	while (Main_Running)
	{
		// Remember the time at the start of the frame.

		std::chrono::time_point<std::chrono::high_resolution_clock> Frame_Start_Time = std::chrono::high_resolution_clock::now();

		// Refresh the dimensions, in case the window was resized.

		SDL_GetWindowSize(Main_Window, &Main_X_Resolution, &Main_Y_Resolution);

		SDL_GL_GetDrawableSize(Main_Window, &Context_X_Resolution, &Context_Y_Resolution);

		Aspect_Ratio = float(Context_X_Resolution) / float(Context_Y_Resolution);

		// Poll and handle events.

		SDL_Event e;

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				// The application was quit.

				Main_Running = false;
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				// The mouse moved.

				if (Main_Mouse_Relative)
				{
					Main_Mouse_X += e.motion.xrel;
					Main_Mouse_Y += e.motion.yrel;

					if (Main_Mouse_Y > Main_Y_Resolution - 1)
					{
						Main_Mouse_Y = Main_Y_Resolution - 1;
					}
					else if (Main_Mouse_Y < 0)
					{
						Main_Mouse_Y = 0;
					}
				}
				else
				{
					Main_Mouse_X = e.motion.x;
					Main_Mouse_Y = e.motion.y;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				// A mouse button was pressed.

				if (e.button.button == SDL_BUTTON_LEFT)
				{
					Main_Mouse_L = true;
				}
				else if (e.button.button == SDL_BUTTON_RIGHT)
				{
					Main_Mouse_R = true;
				}
			}
