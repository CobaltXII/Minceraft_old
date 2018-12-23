#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Load a file as a std::string.

std::string Load_File_As_String(std::string Path)
{
	std::ifstream File(Path);

	if (!File.is_open())
	{
		std::cout << "Could not open file \"" << Path << "\"." << std::endl;

		exit(8);
	}

	std::stringstream Buffer;

	Buffer << File.rdbuf() << "\0";

	return Buffer.str();
}

// Load a shader from a file.

GLuint Load_Shader(std::string Path, GLenum Shader_Type)
{
	GLuint Shader = glCreateShader(Shader_Type);

	std::string File_Contents = Load_File_As_String(Path);

	const GLchar* File_Data = File_Contents.c_str();

	glShaderSource(Shader, 1, &File_Data, NULL);

	glCompileShader(Shader);

	GLint Success = 0;

	glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);

	if (!Success)
	{
		std::cout << "Could not compile shader loaded from \"" << Path << "\"." << std::endl;

		GLchar Crash_Information[4096];

		glGetShaderInfoLog(Shader, 4096, NULL, Crash_Information);

		std::cout << Crash_Information;

		exit(9);
	}

	return Shader;
}