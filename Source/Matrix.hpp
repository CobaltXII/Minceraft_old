// A mat4 is just an array of 16 consecutive floating-point integers.

typedef float* mat4;

// Generate an empty mat4.

mat4 Mat4_Empty()
{
	mat4 Out = (mat4)malloc(16 * sizeof(float));

	for (int i = 0; i < 16; i++)
	{
		Out[i] = 0.0f;
	}

	return Out;
}
