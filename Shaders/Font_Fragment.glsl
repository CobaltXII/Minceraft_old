#version 330 core

// Frame buffer texture.

uniform sampler2D Screen_Texture;

// Input from vertex shader.

in vec2 Texture_Coordinates;

in float Font_Color;

// Output to OpenGL.

out vec4 Fragment_Color;

// Font colors.

vec4 Font_Colors[16] = vec4[16]
(
	vec4(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f),

	vec4(0.0f / 255.0f, 0.0f / 255.0f, 170.0f / 255.0f, 1.0f),

	vec4(0.0f / 255.0f, 170.0f / 255.0f, 0.0f / 255.0f, 1.0f),

	vec4(0.0f / 255.0f, 170.0f / 255.0f, 170.0f / 255.0f, 1.0f),

	vec4(170.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f),

	vec4(170.0f / 255.0f, 0.0f / 255.0f, 170.0f / 255.0f, 1.0f),

	vec4(255.0f / 255.0f, 170.0f / 255.0f, 0.0f / 255.0f, 1.0f),

	vec4(170.0f / 255.0f, 170.0f / 255.0f, 170.0f / 255.0f, 1.0f),

	vec4(85.0f / 255.0f, 85.0f / 255.0f, 85.0f / 255.0f, 1.0f),

	vec4(85.0f / 255.0f, 85.0f / 255.0f, 255.0f / 255.0f, 1.0f),

	vec4(85.0f / 255.0f, 255.0f / 255.0f, 85.0f / 255.0f, 1.0f),

	vec4(85.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),

	vec4(255.0f / 255.0f, 85.0f / 255.0f, 85.0f / 255.0f, 1.0f),

	vec4(255.0f / 255.0f, 85.0f / 255.0f, 255.0f / 255.0f, 1.0f),

	vec4(255.0f / 255.0f, 255.0f / 255.0f, 85.0f / 255.0f, 1.0f),

	vec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f)
);

// Main shader code.

void main()
{
    if (Font_Color > 15)
    {
    	Fragment_Color = texture(Screen_Texture, Texture_Coordinates) * Font_Colors[int(Font_Color - 16)];

    	Fragment_Color = vec4(Fragment_Color.xyz * 0.25f, Fragment_Color.w);
    }
    else
    {
    	Fragment_Color = texture(Screen_Texture, Texture_Coordinates) * Font_Colors[int(Font_Color)];
    }
}