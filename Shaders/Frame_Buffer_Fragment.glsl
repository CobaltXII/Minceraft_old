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

    int Effect = -1;

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
    else if (Effect == 2)
    {
    	// Saturate.

    	vec3 Gray_Tone_Base = vec3(0.3f, 0.59f, 0.11f);

		vec3 Gray_Tone = vec3(dot(Gray_Tone_Base, Fragment_Color.xyz));

		Fragment_Color = vec4(mix(Fragment_Color.xyz, Gray_Tone, -0.5f), 1.0f);
    }
    else if (Effect == 3)
    {
    	// Desaturate.

    	vec3 Gray_Tone_Base = vec3(0.3f, 0.59f, 0.11f);

		vec3 Gray_Tone = vec3(dot(Gray_Tone_Base, Fragment_Color.xyz));

		Fragment_Color = vec4(mix(Fragment_Color.xyz, Gray_Tone, 0.5f), 1.0f);
    }
    else if (Effect == 4)
    {
    	// Grayscale.

    	float Grayscale = Fragment_Color.x * 0.2126f + Fragment_Color.y * 0.7152f + Fragment_Color.z * 0.0722f;

		Fragment_Color = vec4(Grayscale, Grayscale, Grayscale, 1.0f);
    }
    else if (Effect == 5)
    {
    	// Warm grayscale.

    	float Grayscale = Fragment_Color.x * 0.2126f + Fragment_Color.y * 0.7152f + Fragment_Color.z * 0.0722f;

		Fragment_Color = vec4(Grayscale, Grayscale, Grayscale, 1.0f);

		float Temperature = 20.0f / 255.0f;

		Fragment_Color.x += Temperature;
		Fragment_Color.z -= Temperature;
    }
    else if (Effect == 6)
    {
    	// Cool grayscale.

    	float Grayscale = Fragment_Color.x * 0.2126f + Fragment_Color.y * 0.7152f + Fragment_Color.z * 0.0722f;

		Fragment_Color = vec4(Grayscale, Grayscale, Grayscale, 1.0f);

		float Temperature = -20.0f / 255.0f;

		Fragment_Color.x += Temperature;
		Fragment_Color.z -= Temperature;
    }
    else if (Effect == 7)
    {
    	// High tint.

    	float Tint = 20.0f / 255.0f;

		Fragment_Color.y += Tint;
    }
    else if (Effect == 8)
    {
    	// Low tint.

    	float Tint = -20.0f / 255.0f;

		Fragment_Color.y += Tint;
    }
}