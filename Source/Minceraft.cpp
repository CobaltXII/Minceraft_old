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
