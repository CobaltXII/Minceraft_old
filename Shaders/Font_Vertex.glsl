#version 330 core

// Vertex information.

layout (location = 0) in vec2 Default_Screen_Position;

layout (location = 1) in vec2 Default_Texture_Coordinates;

layout (location = 2) in float Default_Font_Color;

// Output to fragment shader.

out vec2 Texture_Coordinates;

out float Font_Color;

// Main shader code.

void main()
{
    gl_Position = vec4(Default_Screen_Position.x, Default_Screen_Position.y, 0.0f, 1.0f);

    Texture_Coordinates = Default_Texture_Coordinates;

    Font_Color = Default_Font_Color;
}