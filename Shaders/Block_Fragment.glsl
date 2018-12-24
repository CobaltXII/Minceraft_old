#version 330 core

// Fog/sky color.

const vec3 Sky_Color = vec3(186.0f / 255.0f, 214.0f / 255.0f, 254.0f / 255.0f);

// The distance from the origin at which the fog density is 1.0f.

uniform float Fog_Distance;

// Texture array.

uniform sampler2DArray Block_Texture_Array;

// Input from vertex shader.

in vec3 Texture_Coordinates;

in float Texture_Brightness;

in float Vertex_Distance;

// Output to OpenGL.

out vec4 Fragment_Color;

// Timestamp.

uniform float Time;

// Main shader code.

void main()
{
	Fragment_Color = texture(Block_Texture_Array, Texture_Coordinates);

	if (Fragment_Color.w == 0.0f)
	{
		// Don't know why we have to do this, but otherwise some faces are not shown.

		discard;
	}

	// Add light.

	Fragment_Color = vec4(Fragment_Color.xyz * Texture_Brightness, Fragment_Color.w);

	// Mix with fog, fog density is based on distance to origin.

	Fragment_Color = vec4(mix(Fragment_Color.xyz, Sky_Color, min(1.0f, pow(Vertex_Distance, 2.0f) / Fog_Distance)), 1.0f);
}