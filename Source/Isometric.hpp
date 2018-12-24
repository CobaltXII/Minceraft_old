// You must call Generate_Isometric_Preliminaries() before doing any isometric block based 
// operations.

GLuint Isometric_Program;

void Generate_Isometric_Preliminaries()
{
	Isometric_Program = Load_Program("../Shaders/Isometric_Vertex.glsl", "../Shaders/Isometric_Fragment.glsl");
}

// Render a mesh that fills a region with an isometric (kind of) model of a specific block type.

void Render_Isometric(int X, int Y, int W, int H, int X_Res, int Y_Res, int Hi_X_Res, int Hi_Y_Res, Block_ID Type, GLuint Block_Texture_Array)
{
	// We need 2 floats to represent the X and Y coordinates of a vertex, and 3 floats to
	// represent the U, V, and W coordinates of a vertex. Finally, we need 1 float to represent
	// the light component of a vertex. So we need 6 floats to completely represent a vertex.

	float Xf = float(X);

	float Yf = float(Y) + float(Y_Res) / float(Hi_Y_Res);

	float Wf = float(W);

	float Hf = float(H) - 2.0f * float(Y_Res) / float(Hi_Y_Res);

	float Neg_X = Xf + Wf / 16.0f;

	float Pos_X = Xf + Wf - Wf / 16.0f;

	float Neg_Y = Yf;

	float Pos_Y = Yf + Hf;

	float Neg_H_Y = Yf + Hf / 142.0f * 32.0f;

	float Pos_H_Y = Yf + Hf / 142.0f * 110.0f;

	float Med_Y = Yf + Hf / 142.0f * 64.0f;

	float Med_X = Xf + Wf / 2.0f;

	Neg_X = Neg_X / float(X_Res) * 2.0f - 1.0f;
	Pos_X = Pos_X / float(X_Res) * 2.0f - 1.0f;

	Neg_Y = Neg_Y / float(Y_Res) * 2.0f - 1.0f;
	Pos_Y = Pos_Y / float(Y_Res) * 2.0f - 1.0f;

	Neg_H_Y = Neg_H_Y / float(Y_Res) * 2.0f - 1.0f;
	Pos_H_Y = Pos_H_Y / float(Y_Res) * 2.0f - 1.0f;

	Med_Y = Med_Y / float(Y_Res) * 2.0f - 1.0f;
	Med_X = Med_X / float(X_Res) * 2.0f - 1.0f;

	float Layer_Top = Block_Face_Info[Type]->Top;

	float Layer_Left = Block_Face_Info[Type]->Left;

	float Layer_Right = Block_Face_Info[Type]->Right;

	float Light_Top = 1.0f;

	float Light_Left = 0.7f;

	float Light_Right = 0.9f;

	// Define vertices.

	float Vertices[] =
	{
		// Top quad.

		Med_X, -Neg_Y, 0.0f, 0.0f, Layer_Top, Light_Top,

		Pos_X, -Neg_H_Y, 1.0f, 0.0f, Layer_Top, Light_Top,

		Neg_X, -Neg_H_Y, 0.0f, 1.0f, Layer_Top, Light_Top,

		Pos_X, -Neg_H_Y, 1.0f, 0.0f, Layer_Top, Light_Top,

		Neg_X, -Neg_H_Y, 0.0f, 1.0f, Layer_Top, Light_Top,

		Med_X, -Med_Y, 1.0f, 1.0f, Layer_Top, Light_Top,

		// Left quad.

		Med_X, -Med_Y, 1.0f, 0.0f, Layer_Left, Light_Left,

		Neg_X, -Neg_H_Y, 0.0f, 0.0f, Layer_Left, Light_Left,

		Neg_X, -Pos_H_Y, 0.0f, 1.0f, Layer_Left, Light_Left,

		Med_X, -Med_Y, 1.0f, 0.0f, Layer_Left, Light_Left,

		Neg_X, -Pos_H_Y, 0.0f, 1.0f, Layer_Left, Light_Left,

		Med_X, -Pos_Y, 1.0f, 1.0f, Layer_Left, Light_Left,

		// Right quad.

		Pos_X, -Neg_H_Y, 1.0f, 0.0f, Layer_Right, Light_Right,

		Med_X, -Med_Y, 0.0f, 0.0f, Layer_Right, Light_Right,

		Med_X, -Pos_Y, 0.0f, 1.0f, Layer_Right, Light_Right,

		Pos_X, -Neg_H_Y, 1.0f, 0.0f, Layer_Right, Light_Right,

		Med_X, -Pos_Y, 0.0f, 1.0f, Layer_Right, Light_Right,

		Pos_X, -Pos_H_Y, 1.0f, 1.0f, Layer_Right, Light_Right
	};

	// Generate VAO and VBO.

	GLuint Isometric_VAO;
	GLuint Isometric_VBO;

	glGenVertexArrays(1, &Isometric_VAO);

	glGenBuffers(1, &Isometric_VBO);

	glBindVertexArray(Isometric_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, Isometric_VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));

	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Render VAO and VBO.

	glUseProgram(Isometric_Program);

	glBindVertexArray(Isometric_VAO);

	glBindTexture(GL_TEXTURE_2D_ARRAY, Block_Texture_Array);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLES, 0, sizeof(Vertices) / sizeof(float) / 6);

	glDisable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	glBindVertexArray(0);

	glUseProgram(0);

	// Delete the VAO and VBO.

	glDeleteVertexArrays(1, &Isometric_VAO);

	glDeleteBuffers(1, &Isometric_VBO);
}