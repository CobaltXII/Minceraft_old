#include <iostream>
#include <string>

// Compiles a shader program after loading two shaders from files. Deletes the two source shaders
// after compilation has finished.

GLuint Load_Program(std::string Vertex_Path, std::string Fragment_Path)
{
	GLuint Shader_Program = glCreateProgram();

	GLuint Vertex_Shader = Load_Shader(Vertex_Path, GL_VERTEX_SHADER);

	GLuint Fragment_Shader = Load_Shader(Fragment_Path, GL_FRAGMENT_SHADER);

	glAttachShader(Shader_Program, Vertex_Shader);

	glAttachShader(Shader_Program, Fragment_Shader);

	glLinkProgram(Shader_Program);

	GLint Success = 0;

	glGetProgramiv(Shader_Program, GL_LINK_STATUS, &Success);

	if (!Success)
	{
		std::cout << "Could not link shader program loaded from \"" << Vertex_Path << "\" and \"" << Fragment_Path << "\"." << std::endl;

		GLchar Crash_Information[4096];

		glGetProgramInfoLog(Shader_Program, 4096, NULL, Crash_Information);

		std::cout << Crash_Information;

		exit(10);
	}

	glDeleteShader(Vertex_Shader);

	glDeleteShader(Fragment_Shader);

	return Shader_Program;
}