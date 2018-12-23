#version 330 core

// Vertex information.

layout (location = 0) in vec2 Screen_Position;

layout (location = 1) in vec2 Screen_Texture;

// Output to fragment shader.

out vec2 Texture_Coordinates;

// Main shader code.

void main()
{
    gl_Position = vec4(Screen_Position.x, Screen_Position.y, 0.0f, 1.0f);

    Texture_Coordinates = Screen_Texture;
}