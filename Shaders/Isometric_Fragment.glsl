#version 330 core

// Texture array.

uniform sampler2DArray Block_Texture_Array;

// Input from vertex shader.

in vec3 Texture_Coordinates;

in float Texture_Brightness;

// Output to OpenGL.

out vec4 Fragment_Color;

// Main shader code.

void main()
{
    Fragment_Color = texture(Block_Texture_Array, Texture_Coordinates);

    if (Fragment_Color.w == 0.0f)
	{
		// Don't know why we have to do this, but otherwise some faces are not shown.

		discard;
	}

	Fragment_Color = vec4(Fragment_Color.xyz * Texture_Brightness * 1.05f, Fragment_Color.w);
}