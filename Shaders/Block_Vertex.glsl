#version 330 core

// Vertex information.

layout (location = 0) in vec3 Default_Position;

layout (location = 1) in vec3 Default_Texture_Coordinates;

layout (location = 2) in float Default_Texture_Brightness;

// Output to fragment shader.

out vec3 Texture_Coordinates;

out float Texture_Brightness;

out float Vertex_Distance;

// Matrices.

uniform mat4 Matrix_Projection;

uniform mat4 Matrix_View_X;
uniform mat4 Matrix_View_Y;

// Translation vector.

uniform vec3 Translation_Vector;

// Timestamp.

uniform float Time;

// Main shader code.

void main()
{
	vec3 New_Position = vec3(Default_Position.x + Translation_Vector.x, -Default_Position.y - Translation_Vector.y, Default_Position.z + Translation_Vector.z);

	gl_Position = vec4(New_Position, 1.0f) * Matrix_View_Y * Matrix_View_X * Matrix_Projection;

	Texture_Coordinates = Default_Texture_Coordinates;

	Texture_Brightness = Default_Texture_Brightness;

	Vertex_Distance = length(New_Position);
}