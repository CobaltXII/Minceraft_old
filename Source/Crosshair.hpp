struct Crosshair
{
	GLuint VAO;
	GLuint VBO;

	unsigned int Vertices;
};

// Generate a Crosshair object representing a crosshair.

Crosshair* Make_Crosshair(int X_Res, int Y_Res)
{
	// Define the size of the crosshair.

	float Long = 9;

	float Short = 1;

	// Define the coordinates in pixel space.

	float Vertical_Neg_X = X_Res / 2 - Short;
	float Vertical_Pos_X = X_Res / 2 + Short;

	float Vertical_T_Y = Y_Res / 2 - Long;
	float Vertical_B_Y = Y_Res / 2 + Long;

	float Horizontal_Neg_Y = Y_Res / 2 - Short;
	float Horizontal_Pos_Y = Y_Res / 2 + Short;

	float Horizontal_L_X = X_Res / 2 - Long;
	float Horizontal_R_X = X_Res / 2 + Long;

	// Make the VBO (locally) in pixel space.

	float Crosshair_Local[] =
	{
		// Top rectangle.

		Vertical_Neg_X, Vertical_T_Y, 
		Vertical_Pos_X, Vertical_T_Y,

		Vertical_Neg_X, Horizontal_Neg_Y,

		Vertical_Pos_X, Horizontal_Neg_Y,
		Vertical_Neg_X, Horizontal_Neg_Y,

		Vertical_Pos_X, Vertical_T_Y,

		// Bottom rectangle.

		Vertical_Neg_X, Vertical_B_Y,

		Vertical_Neg_X, Horizontal_Pos_Y,
		Vertical_Pos_X, Horizontal_Pos_Y,

		Vertical_Pos_X, Vertical_B_Y,
		Vertical_Neg_X, Vertical_B_Y,

		Vertical_Pos_X, Horizontal_Pos_Y,

		// Left rectangle.

		Horizontal_L_X, Horizontal_Pos_Y, 
		Horizontal_L_X, Horizontal_Neg_Y,

		Vertical_Neg_X, Horizontal_Neg_Y,

		Vertical_Neg_X, Horizontal_Neg_Y,
		Vertical_Neg_X, Horizontal_Pos_Y,

		Horizontal_L_X, Horizontal_Pos_Y,

		// Right rectangle.

		Vertical_Pos_X, Horizontal_Pos_Y,
		Vertical_Pos_X, Horizontal_Neg_Y,

		Horizontal_R_X, Horizontal_Neg_Y,

		Horizontal_R_X, Horizontal_Neg_Y,
		Horizontal_R_X, Horizontal_Pos_Y,

		Vertical_Pos_X, Horizontal_Pos_Y,

		// Center rectangle.

		Vertical_Neg_X, Horizontal_Pos_Y,
		Vertical_Neg_X, Horizontal_Neg_Y,

		Vertical_Pos_X, Horizontal_Neg_Y,

		Vertical_Pos_X, Horizontal_Neg_Y,
		Vertical_Pos_X, Horizontal_Pos_Y,

		Vertical_Neg_X, Horizontal_Pos_Y
	};

	// Convert this into normalized screen coordinates.

	float* Crosshair_Global = (float*)malloc(sizeof(Crosshair_Local));

	for (int i = 0; i < sizeof(Crosshair_Local) / sizeof(float); i += 2)
	{
		Crosshair_Global[i + 0] = Crosshair_Local[i + 0] / float(X_Res) * 2.0f - 1.0f;
		Crosshair_Global[i + 1] = Crosshair_Local[i + 1] / float(Y_Res) * 2.0f - 1.0f;
	}

	// Generate a VBO and a VAO.

	GLuint Target_VBO;
	GLuint Target_VAO;

	glGenVertexArrays(1, &Target_VAO);

	glGenBuffers(1, &Target_VBO);

	// Bind the VBO and the VAO to the current state.

	glBindVertexArray(Target_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, Target_VBO);

	// Upload the target data to the GPU.

	glBufferData(GL_ARRAY_BUFFER, sizeof(Crosshair_Local), Crosshair_Global, GL_STATIC_DRAW);

	// Free the data from the CPU, as it's on the GPU now.

	free(Crosshair_Global);

	// Enable the default vertex attribute pointers.

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0 * sizeof(float)));

	glEnableVertexAttribArray(0);

	// Unbind the VBO and the VAO from the current state.

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Generate a Crosshair object.

	Crosshair* Out = new Crosshair();

	Out->VBO = Target_VBO;
	Out->VAO = Target_VAO;

	Out->Vertices = sizeof(Crosshair_Local) / sizeof(float) / 2;

	return Out;
}

// Destroy a Crosshair.

void Destroy_Crosshair(Crosshair* The_Crosshair)
{
	glDeleteVertexArrays(1, &The_Crosshair->VAO);

	glDeleteBuffers(1, &The_Crosshair->VBO);

	delete The_Crosshair;
}