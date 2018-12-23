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

// Set a matrix to an identity matrix.

mat4 Mat4_Identity(mat4 Out)
{
	for (int i = 0; i < 16; i++)
	{
		Out[i] = 0.0f;
	}

	for (int i = 0; i < 4; i++)
	{
		Out[i * 4 + i] = 1.0f;
	}

	return Out;
}

// Set a matrix to a projection matrix.

mat4 Mat4_Projection(mat4 Out, float Field_Of_View, float Aspect_Ratio, float Z_Near, float Z_Far)
{
	for (int i = 0; i < 16; i++)
	{
		Out[i] = 0.0f;
	}

	float Tangent_Half_Field_Of_View = tan(Field_Of_View / 2.0f);

	Out[0 * 4 + 0] = 1.0f / (Aspect_Ratio * Tangent_Half_Field_Of_View);

	Out[1 * 4 + 1] = 1.0f / Tangent_Half_Field_Of_View;

	Out[2 * 4 + 2] = -(Z_Far + Z_Near) / (Z_Far - Z_Near);

	Out[2 * 4 + 3] = -1.0f;

	Out[3 * 4 + 2] = -(2.0 * Z_Far * Z_Near) / (Z_Far - Z_Near);

	return Out;
}

// Set a matrix to a X rotation matrix.

mat4 Mat4_Rotate_X(mat4 Out, float Radians)
{
	float Cos = cos(Radians);
	float Sin = sin(Radians);

	for (int i = 0; i < 16; i++)
	{
		Out[i] = 0.0f;
	}

	// Line 1.

	Out[0] = 1.0f;
	Out[1] = 0.0f;
	Out[2] = 0.0f;
	Out[3] = 0.0f;

	// Line 2.

	Out[4] = 0.0f;

	Out[5] = Cos;
	Out[6] = Sin;

	Out[7] = 0.0f;

	// Line 3.

	Out[8] = 0.0f;

	Out[9] = -Sin;

	Out[10] = Cos;

	Out[11] = 0.0f;

	// Line 4.

	Out[12] = 0.0f;
	Out[13] = 0.0f;
	Out[14] = 0.0f;
	Out[15] = 1.0f;

	return Out;
}

// Set a matrix to a Y rotation matrix.

mat4 Mat4_Rotate_Y(mat4 Out, float Radians)
{
	float Cos = cos(Radians);
	float Sin = sin(Radians);

	for (int i = 0; i < 16; i++)
	{
		Out[i] = 0.0f;
	}

	// Line 1.

	Out[0] = Cos;

	Out[1] = 0.0f;

	Out[2] = -Sin;

	Out[3] = 0.0f;

	// Line 2.

	Out[4] = 0.0f;
	Out[5] = 1.0f;
	Out[6] = 0.0f;
	Out[7] = 0.0f;

	// Line 3.

	Out[8] = Sin;

	Out[9] = 0.0f;

	Out[10] = Cos;

	Out[11] = 0.0f;

	// Line 4.

	Out[12] = 0.0f;
	Out[13] = 0.0f;
	Out[14] = 0.0f;
	Out[15] = 1.0f;

	return Out;
}

// Degrees to radians.

inline float Radians(float Degrees)
{
	return Degrees * M_PI / 180.0f;
}