// You can't use this for general purpose stuff! It only works for what I use it for.

struct Frame_Buffer
{
	GLuint The_Frame_Buffer;

	GLuint The_Render_Buffer;

	GLuint The_Quad;

	GLuint The_Texture;
};

// Build a frame buffer.

Frame_Buffer* Build_Frame_Buffer(int X_Res, int Y_Res)
{
	// Generate a VAO and VBO representing a quad to render the framebuffer to.

	float Quad_Vertices[] = 
	{
        -1.0f, +1.0f, +0.0f, +1.0f,
        -1.0f, -1.0f, +0.0f, +0.0f,
        +1.0f, -1.0f, +1.0f, +0.0f,

        -1.0f, +1.0f, +0.0f, +1.0f,
        +1.0f, -1.0f, +1.0f, +0.0f,
        +1.0f, +1.0f, +1.0f, +1.0f
    };

	GLuint Quad_VAO;
	GLuint Quad_VBO;

	glGenVertexArrays(1, &Quad_VAO);

	glGenBuffers(1, &Quad_VBO);

	glBindVertexArray(Quad_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, Quad_VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Quad_Vertices), Quad_Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(float)));

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Generate the frame buffer (this is used for post-processing).

	GLuint Frame_Buffer_Thing;

	glGenFramebuffers(1, &Frame_Buffer_Thing);

	glBindFramebuffer(GL_FRAMEBUFFER, Frame_Buffer_Thing);

	// Create a texture to attach to the frame buffer.

	GLuint Frame_Buffer_Texture;

	glGenTextures(1, &Frame_Buffer_Texture);

	glBindTexture(GL_TEXTURE_2D, Frame_Buffer_Texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, X_Res, Y_Res, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Frame_Buffer_Texture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Create a render buffer object to hold the depth buffer.

	GLuint Render_Buffer;

	glGenRenderbuffers(1, &Render_Buffer);

	glBindRenderbuffer(GL_RENDERBUFFER, Render_Buffer);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, X_Res, Y_Res);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, Render_Buffer);

	// Make sure the frame buffer is complete.

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Could not complete the initialization of a GL_FRAMEBUFFER." << std::endl;

		exit(12);
	}

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Generate the storage class.

    Frame_Buffer* The_Frame_Buffer = new Frame_Buffer();

    The_Frame_Buffer->The_Frame_Buffer = Frame_Buffer_Thing;

    The_Frame_Buffer->The_Render_Buffer = Render_Buffer;

    The_Frame_Buffer->The_Quad = Quad_VAO;

    The_Frame_Buffer->The_Texture = Frame_Buffer_Texture;

    return The_Frame_Buffer;
}

// Rebuild the entire frame buffer.

void Rebuild_Frame_Buffer(Frame_Buffer* The_Frame_Buffer, int X_Res, int Y_Res)
{
	// Destroy old contents.

	glDeleteRenderbuffers(1, &The_Frame_Buffer->The_Render_Buffer);

	glDeleteTextures(1, &The_Frame_Buffer->The_Texture);

	glDeleteFramebuffers(1, &The_Frame_Buffer->The_Frame_Buffer);

	// Generate the frame buffer (this is used for post-processing).

	GLuint Frame_Buffer_Thing;

	glGenFramebuffers(1, &Frame_Buffer_Thing);

	glBindFramebuffer(GL_FRAMEBUFFER, Frame_Buffer_Thing);

	// Create a texture to attach to the frame buffer.

	GLuint Frame_Buffer_Texture;

	glGenTextures(1, &Frame_Buffer_Texture);

	glBindTexture(GL_TEXTURE_2D, Frame_Buffer_Texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, X_Res, Y_Res, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Frame_Buffer_Texture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Create a render buffer object to hold the depth buffer.

	GLuint Render_Buffer;

	glGenRenderbuffers(1, &Render_Buffer);

	glBindRenderbuffer(GL_RENDERBUFFER, Render_Buffer);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, X_Res, Y_Res);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, Render_Buffer);

	// Make sure the frame buffer is complete.

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Could not complete the initialization of a GL_FRAMEBUFFER." << std::endl;

		exit(15);
	}

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Generate the storage class.

    The_Frame_Buffer->The_Frame_Buffer = Frame_Buffer_Thing;

    The_Frame_Buffer->The_Render_Buffer = Render_Buffer;

    The_Frame_Buffer->The_Texture = Frame_Buffer_Texture;
}