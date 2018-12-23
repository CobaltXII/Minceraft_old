#version 330 core

// Frame buffer texture.

uniform sampler2D Screen_Texture;

// Input from vertex shader.

in vec2 Texture_Coordinates;

// Output to OpenGL.

out vec4 Fragment_Color;

// Main shader code.

void main()
{
    Fragment_Color = texture(Screen_Texture, Texture_Coordinates);

    int Effect = 0;

    if (Effect == 0)
    {
    	// Warm.

    	float Temperature = 20.0f / 255.0f;

		Fragment_Color.x += Temperature;
		Fragment_Color.z -= Temperature;
    }
    else if (Effect == 1)
    {
    	// Cool.

    	float Temperature = -20.0f / 255.0f;

		Fragment_Color.x += Temperature;
		Fragment_Color.z -= Temperature;
    }
