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

