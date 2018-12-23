#version 330 core

// Vertex information.

layout (location = 0) in vec3 Default_Position;

// Main shader code.

void main()
{
	gl_Position = vec4(Default_Position, 1.0f);
}