#include <SDL2/SDL.h>

#define GL_SILENCE_DEPRECATION

#include <OpenGL/GL3.h>

#define STB_IMAGE_IMPLEMENTATION

#include "../Libraries/stb_image.hpp"

#include "Minceraft.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

#include <cmath>

// Format a size in bytes.

std::string Byte_Format(unsigned int Bytes)
{
	std::stringstream Format;

	if (Bytes > 1024 * 1024 * 1024)
	{
		Format << float(Bytes) / float(1024 * 1024 * 1024) << " GB";
	}
	else if (Bytes > 1024 * 1024)
	{
		Format << float(Bytes) / float(1024 * 1024) << " MB";
	}
	else if (Bytes > 1024)
	{
		Format << float(Bytes) / float(1024) << " kB";
	}
	else
	{
		Format << float(Bytes) << " B";
	}

	return Format.str();
}

// Current Minceraft version.

std::string Minceraft_Version = "0.0.64";

// Generate the version string for this build of Minceraft.

std::string Make_Minceraft_Version_String()
{
	std::stringstream String_Stream;

	String_Stream << "Minceraft " << Minceraft_Version;

	return String_Stream.str();
}

// Requested dimensions.

int Main_X_Resolution;
int Main_Y_Resolution;

// Actual dimensions.

int Context_X_Resolution;
int Context_Y_Resolution;

// Aspect ratio.

float Aspect_Ratio = 1.0;

// Mouse position.

int Main_Mouse_X = 0;
int Main_Mouse_Y = 0;

// Mouse state.

bool Main_Mouse_L = false;
bool Main_Mouse_R = false;

bool Main_Mouse_Relative = false;

// Iteration counter.

unsigned long Main_Iteration = 0;

// Entry point.

int main(int argc, char** argv)
{
	// Initialize dimensions.

	Main_X_Resolution = 950;
	Main_Y_Resolution = 540;

	// Initialize SDL.

	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		std::cout << "Could not initialize SDL." << std::endl;

		exit(1);
	}

	// Create the SDL_Window.

	SDL_Window* Main_Window = SDL_CreateWindow
	(
		Make_Minceraft_Version_String().c_str(),

		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,

		Main_X_Resolution,
		Main_Y_Resolution,

		SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL
	);

	// Check if the SDL_Window was created successfully.

	if (!Main_Window)
	{
		std::cout << "Could not create the SDL_Window." << std::endl;

		exit(2);
	}

	// Request OpenGL 3.2.

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Request double-buffering.

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create the SDL_GLContext.

	SDL_GLContext Main_Context = SDL_GL_CreateContext(Main_Window);

	if (!Main_Context)
	{
		std::cout << "Could not create the SDL_GLContext." << std::endl;

		exit(3);
	}

	// Set the swap interval to update immediately.

	SDL_GL_SetSwapInterval(0);

	// Set up preliminaries for depth testing.

	glEnable(GL_DEPTH_CLAMP);

	// Set up preliminaries for backface culling.

	glCullFace(GL_BACK);

	glFrontFace(GL_CW);

	// Get the dimensions and aspect ratio of the OpenGL context.

	SDL_GL_GetDrawableSize(Main_Window, &Context_X_Resolution, &Context_Y_Resolution);

	Aspect_Ratio = float(Context_X_Resolution) / float(Context_Y_Resolution);

	// Load the texture array that contains every block texture.

	GLuint Block_Texture_Array = Generate_Block_Texture_Array();

	Make_Block_Face_Info();

	// Load the interface textures.

	Generate_Interface_Textures();

	// Generate sprite and isometric block preliminaries. Basically loads the sprite and isometric shaders.

	Generate_Sprite_Preliminaries();

	Generate_Isometric_Preliminaries();

	// Load the block shader program.

	GLuint Block_Program = Load_Program("../Shaders/Block_Vertex.glsl", "../Shaders/Block_Fragment.glsl");

	// Load the crosshair shader program.

	GLuint Crosshair_Program = Load_Program("../Shaders/Crosshair_Vertex.glsl", "../Shaders/Crosshair_Fragment.glsl");

	// Load the frame buffer shader program.

	GLuint Frame_Buffer_Program = Load_Program("../Shaders/Frame_Buffer_Vertex.glsl", "../Shaders/Frame_Buffer_Fragment.glsl");

	// Generate the frame buffer.

	Frame_Buffer* The_Frame_Buffer = Build_Frame_Buffer(Context_X_Resolution, Context_Y_Resolution);

	// Generate the projection matrix.

	mat4 Matrix_Projection = Mat4_Empty();

	Mat4_Projection(Matrix_Projection, Radians(70.0f), Aspect_Ratio, 1.0f, 1000.0f);

	// Generate the view matrices.

	mat4 Matrix_View_X = Mat4_Empty();
	mat4 Matrix_View_Y = Mat4_Empty();

	Mat4_Rotate_X(Matrix_View_X, 0.0f);
	Mat4_Rotate_Y(Matrix_View_Y, 0.0f);

	// Generate a fresh world.

	int World_Size = Id_Medium_World;

	World* The_World = Allocate_Empty_World(World_X_Res_List[World_Size], World_Y_Res_List[World_Size], World_Z_Res_List[World_Size]);

	Generate_World(The_World, time(NULL));

	// Generate a segmenter to represent the world.

	Segmenter* The_Segmenter = Make_Segmenter(The_World);

	// Show the window.

	SDL_ShowWindow(Main_Window);

	// Create variables to hold the player's position.

	float Player_X = -float(The_World->X_Res) / 2.0f;

	float Player_Y = 0.0f;

	float Player_Z = -float(The_World->Z_Res) / 2.0f;

	// Create variables to hold the player's velocity.

	float Player_Vx = 0.0f;
	float Player_Vy = 0.0f;
	float Player_Vz = 0.0f;

	// Create variables to hold the player's direction.

	float Look_X = 0.0f;
	float Look_Y = 0.0f;

	// Create a variable to hold the player's selected item.

	int Player_Selection = 0;

	// Enter the main loop.

	bool Main_Running = true;

	while (Main_Running)
	{
		// Remember the time at the start of the frame.

		std::chrono::time_point<std::chrono::high_resolution_clock> Frame_Start_Time = std::chrono::high_resolution_clock::now();

		// Refresh the dimensions, in case the window was resized.

		SDL_GetWindowSize(Main_Window, &Main_X_Resolution, &Main_Y_Resolution);

		SDL_GL_GetDrawableSize(Main_Window, &Context_X_Resolution, &Context_Y_Resolution);

		Aspect_Ratio = float(Context_X_Resolution) / float(Context_Y_Resolution);

		// Poll and handle events.

		SDL_Event e;

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				// The application was quit.

				Main_Running = false;
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				// The mouse moved.

				if (Main_Mouse_Relative)
				{
					Main_Mouse_X += e.motion.xrel;
					Main_Mouse_Y += e.motion.yrel;

					if (Main_Mouse_Y > Main_Y_Resolution - 1)
					{
						Main_Mouse_Y = Main_Y_Resolution - 1;
					}
					else if (Main_Mouse_Y < 0)
					{
						Main_Mouse_Y = 0;
					}
				}
				else
				{
					Main_Mouse_X = e.motion.x;
					Main_Mouse_Y = e.motion.y;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				// A mouse button was pressed.

				if (e.button.button == SDL_BUTTON_LEFT)
				{
					Main_Mouse_L = true;
				}
				else if (e.button.button == SDL_BUTTON_RIGHT)
				{
					Main_Mouse_R = true;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				// A mouse button was released.

				if (e.button.button == SDL_BUTTON_LEFT)
				{
					Main_Mouse_L = false;
				}
				else if (e.button.button == SDL_BUTTON_RIGHT)
				{
					Main_Mouse_R = false;
				}
			}
			else if (e.type == SDL_KEYDOWN)
			{
				// A key was pressed.

				SDL_Keycode Key = e.key.keysym.sym;

				if (Key == SDLK_ESCAPE)
				{
					if (Main_Mouse_Relative)
					{
						SDL_SetRelativeMouseMode(SDL_FALSE);

						Main_Mouse_Relative = false;
					}
					else
					{
						Main_Running = false;
					}
				}
				else if (Key == SDLK_1)
				{
					Player_Selection = 0;
				}
				else if (Key == SDLK_2)
				{
					Player_Selection = 1;
				}
				else if (Key == SDLK_3)
				{
					Player_Selection = 2;
				}
				else if (Key == SDLK_4)
				{
					Player_Selection = 3;
				}
				else if (Key == SDLK_5)
				{
					Player_Selection = 4;
				}
				else if (Key == SDLK_6)
				{
					Player_Selection = 5;
				}
				else if (Key == SDLK_7)
				{
					Player_Selection = 6;
				}
				else if (Key == SDLK_8)
				{
					Player_Selection = 7;
				}
				else if (Key == SDLK_9)
				{
					Player_Selection = 8;
				}
			}
			else if (e.type == SDL_WINDOWEVENT)
			{
				if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					SDL_GL_GetDrawableSize(Main_Window, &Context_X_Resolution, &Context_Y_Resolution);

					Rebuild_Frame_Buffer(The_Frame_Buffer, Context_X_Resolution, Context_Y_Resolution);
				}
			}
		}

		// Set relative mouse mode, if required.

		if (Main_Mouse_L)
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);

			Main_Mouse_Relative = true;
		}

		// Calculate the looking direction.

		float Want_Look_X = (float(Main_Mouse_Y) - (float(Main_Y_Resolution) / 2.0f)) / float(Main_Y_Resolution) * -180.0f;

		Look_X += (Want_Look_X - Look_X) / 16.0f;

		if (Look_X < -90.0f)
		{
			Look_X = -90.0f;
		}

		if (Look_X > 90.0f)
		{
			Look_X = 90.0f;
		}

		float Want_Look_Y = (float(Main_Mouse_X) - (float(Main_X_Resolution) / 2.0f)) / float(Main_X_Resolution) * -360.0f;

		Look_Y += (Want_Look_Y - Look_Y) / 16.0f;

		// Calculate looking matrices.

		Mat4_Rotate_X(Matrix_View_X, Radians(Look_X));
		Mat4_Rotate_Y(Matrix_View_Y, Radians(Look_Y));

		// Calculate the projection matrix.

		Mat4_Projection(Matrix_Projection, Radians(70.0f), Aspect_Ratio, 1.0f, 100.0f);

		// Get the keyboard state.

		const Uint8* Keys = SDL_GetKeyboardState(NULL);

		// Accelerate the player's velocity.

		float Acceleration = 0.015f;

		if (Keys[SDL_SCANCODE_W])
		{
			Player_Vz += cos(Radians(Look_Y)) * Acceleration;
			Player_Vx += sin(Radians(Look_Y)) * Acceleration;

			// Yeah, this call to sin is actually a mistake, but it works better than a call to
			// tan, funnily enough.

			Player_Vy += sin(Radians(Look_X)) * Acceleration;
		}
		else if (Keys[SDL_SCANCODE_S])
		{
			Player_Vz -= cos(Radians(Look_Y)) * Acceleration;
			Player_Vx -= sin(Radians(Look_Y)) * Acceleration;

			Player_Vy -= sin(Radians(Look_X)) * Acceleration;
		}

		if (Keys[SDL_SCANCODE_A])
		{
			Player_Vz += cos(Radians(Look_Y + 90.0f)) * Acceleration;
			Player_Vx += sin(Radians(Look_Y + 90.0f)) * Acceleration;
		}
		else if (Keys[SDL_SCANCODE_D])
		{
			Player_Vz += cos(Radians(Look_Y - 90.0f)) * Acceleration;
			Player_Vx += sin(Radians(Look_Y - 90.0f)) * Acceleration;
		}

		if (Keys[SDL_SCANCODE_SPACE])
		{
			Player_Vy += Acceleration;
		}
		else if (Keys[SDL_SCANCODE_LSHIFT])
		{
			Player_Vy -= Acceleration;
		}

		// Update the player's velocity and position.

		float Friction = 0.9f;

		Player_Vx *= Friction;
		Player_Vy *= Friction;
		Player_Vz *= Friction;

		Player_X += Player_Vx;
		Player_Y += Player_Vy;
		Player_Z += Player_Vz;

		// Use the new frame buffer as a render target. This is used instead of the default render
		// target so that post-processing effects can be applied easily.

		glBindFramebuffer(GL_FRAMEBUFFER, The_Frame_Buffer->The_Frame_Buffer);

		// Clear screen to the sky color.

		glClearColor(186.0f / 255.0f, 214.0f / 255.0f, 254.0f / 255.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// The following code handles world rendering.

		{
			// Enable depth testing.

			glEnable(GL_DEPTH_TEST);

			// Enable backface culling.

			glEnable(GL_CULL_FACE);

			// Enable alpha blending.

			glEnable(GL_BLEND);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// Enable the block shader program.

			glUseProgram(Block_Program);

			// Activate the block texture array.

			glBindTexture(GL_TEXTURE_2D_ARRAY, Block_Texture_Array);

			// Pass the uniform variables to the block shader program.

			glUniform1f(glGetUniformLocation(Block_Program, "Time"), float(SDL_GetTicks()) / 1000.0f);

			glUniform1i(glGetUniformLocation(Block_Program, "Block_Texture_Array"), 0);

			glUniformMatrix4fv(glGetUniformLocation(Block_Program, "Matrix_Projection"), 1, GL_FALSE, Matrix_Projection);

			glUniformMatrix4fv(glGetUniformLocation(Block_Program, "Matrix_View_X"), 1, GL_FALSE, Matrix_View_X);
			glUniformMatrix4fv(glGetUniformLocation(Block_Program, "Matrix_View_Y"), 1, GL_FALSE, Matrix_View_Y);

			glUniform3f(glGetUniformLocation(Block_Program, "Translation_Vector"), Player_X, Player_Y, Player_Z);

			// Fog uses a special algorithm to calculate it's far distance.

			glUniform1f(glGetUniformLocation(Block_Program, "Fog_Distance"), pow(sqrt(float(The_World->X_Res * The_World->Z_Res)), 1.5f));

			// Block destruction.

			if (false)
			{
				float Px = -Player_X;
				float Py = -Player_Y;
				float Pz = -Player_Z;

				float Ix = -sin(Radians(Look_Y)) * 0.2f;

				float Iy = -tan(Radians(Look_X)) * 0.2f;

				float Iz = -cos(Radians(Look_Y)) * 0.2f;

				while (true)
				{
					// Check for collisions.

					if (Voxel_Type(The_World->Get_Safe(int(Px), int(Py), int(Pz))) != id_air)
					{
						The_Segmenter->Set_Safe_Unlit(int(Px), int(Py), int(Pz), id_air);
					}

					// Increment ray position.

					Px += Ix;
					Py += Iy;
					Pz += Iz;

					// Abort when too far.

					if ((Px + Player_X) * (Px + Player_X) + (Py + Player_Y) * (Py + Player_Y) + (Pz + Player_Z) * (Pz + Player_Z) > 16.0f * 16.0f)
					{
						break;
					}
				}
			}

			if (Main_Mouse_L)
			{
				int Radius = 3;

				for (int X = -Radius; X <= Radius; X++)
				{
					for (int Y = -Radius; Y <= Radius; Y++)
					{
						for (int Z = -Radius; Z <= Radius; Z++)
						{
							if (X * X + Y * Y + Z * Z < Radius * Radius)
							{
								The_Segmenter->Set_Safe_Unlit(-Player_X + X, -Player_Y + Y, -Player_Z + Z, id_white_wool);
							}
						}
					}
				}
			}
			else if (Main_Mouse_R)
			{
				int Radius = 16;

				for (int X = -Radius; X <= Radius; X++)
				{
					for (int Y = -Radius; Y <= Radius; Y++)
					{
						for (int Z = -Radius; Z <= Radius; Z++)
						{
							if (X * X + Y * Y + Z * Z < Radius * Radius)
							{
								The_Segmenter->Set_Safe_Unlit(-Player_X + X, -Player_Y + Y, -Player_Z + Z, id_air);
							}
						}
					}
				}
			}

			// Draw and update the chunks. We could iterate using a 1-dimensional loop, but we want
			// to update the topmost chunks first, so that light propagates correctly.

			unsigned int Chunk_Updates = 0;

			for (int X = 0; X < The_Segmenter->Chunk_X_Res; X++)
			{
				for (int Z = 0; Z < The_Segmenter->Chunk_Z_Res; Z++)
				{
					for (int Y = 0; Y < The_Segmenter->Chunk_Y_Res; Y++)
					{
						Chunk*& The_Chunk = The_Segmenter->The_Chunks[X + The_Segmenter->Chunk_X_Res * (Y + The_Segmenter->Chunk_Y_Res * Z)];

						if (The_Chunk->Modified)
						{
							The_Chunk->Destroy();

							Update_Subset(The_Segmenter, The_Chunk->X, The_Chunk->Y, The_Chunk->Z, The_Chunk->X_Res, The_Chunk->Y_Res, The_Chunk->Z_Res);

							Propagate_Skylight(The_World, The_Chunk->X, The_Chunk->Y, The_Chunk->Z, The_Chunk->X_Res, The_Chunk->Y_Res, The_Chunk->Z_Res);

							Chunk* New_Chunk = Make_Chunk(The_World, The_Chunk->X, The_Chunk->Y, The_Chunk->Z, The_Chunk->X_Res, The_Chunk->Y_Res, The_Chunk->Z_Res);

							free(The_Chunk);

							The_Chunk = New_Chunk;

							Chunk_Updates++;
						}

						Draw_Chunk(The_Chunk);
					}
				}
			}

			// Draw water layers.

			glDepthMask(GL_FALSE);

			for (int X = 0; X < The_Segmenter->Chunk_X_Res; X++)
			{
				for (int Z = 0; Z < The_Segmenter->Chunk_Z_Res; Z++)
				{
					for (int Y = 0; Y < The_Segmenter->Chunk_Y_Res; Y++)
					{
						Draw_Chunk_Water(The_Segmenter->The_Chunks[X + The_Segmenter->Chunk_X_Res * (Y + The_Segmenter->Chunk_Y_Res * Z)]);
					}
				}
			}

			glDepthMask(GL_TRUE);			

			std::cout << Chunk_Updates << " chunk update(s).\r" << std::flush;

			// Deactivate the block shader program.

			glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

			// Disable the block shader program.

			glUseProgram(0);

			// Disable alpha blending.

			glDisable(GL_BLEND);

			// Disable backface culling.

			glDisable(GL_CULL_FACE);

			// Disable depth testing.

			glDisable(GL_DEPTH_TEST);
		}

		// The following code handles crosshair rendering.

		if (false)
		{
			// Enable alpha blending.

			glEnable(GL_BLEND);

			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);

			// Generate the crosshair.

			Crosshair* The_Crosshair = Make_Crosshair(Main_X_Resolution, Main_Y_Resolution);

			// Enable the crosshair shader program.

			glUseProgram(Crosshair_Program);

			// Bind the vertex array object to the current state.

			glBindVertexArray(The_Crosshair->VAO);

			// Draw the vertex array object as an array of triangles.

			glDrawArrays(GL_TRIANGLES, 0, The_Crosshair->Vertices);

			// Unbind the vertex array object from the current state.

			glBindVertexArray(0);

			// Disable the crosshair shader program.

			glUseProgram(0);

			// Degenerate the crosshair.

			Destroy_Crosshair(The_Crosshair);

			// Disable alpha blending.

			glDisable(GL_BLEND);
		}

		// Draw the heads-up-display.

		{
			// Draw_Hotbar(Main_X_Resolution, Main_Y_Resolution, Context_X_Resolution, Context_Y_Resolution, Player_Selection % 9, Block_Texture_Array, 2);
		}

		// The following code handles frame buffer rendering.

		{
			// Bind to the original frame buffer (the one bound to the window's display).

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// Enable the frame buffer's designated shader program.

			glUseProgram(Frame_Buffer_Program);

			// Bind the quad vertex array to the current state.

			glBindVertexArray(The_Frame_Buffer->The_Quad);

			// Bind the frame buffer's attached texture to the current state.

			glBindTexture(GL_TEXTURE_2D, The_Frame_Buffer->The_Texture);

			// Draw the quad to the original frame buffer.

			glDrawArrays(GL_TRIANGLES, 0, 6);

			// Unbind the frame buffer's attached texture from the current state.

			glBindTexture(GL_TEXTURE_2D, 0);

			// Unbind the quad vertex array from the current state.

			glBindVertexArray(0);

			// Disable the frame buffer's designated shader program.

			glUseProgram(0);
		}

		// Draw frame.

		SDL_GL_SwapWindow(Main_Window);

		// Cap framerate.

		double Frame_Elapsed_Time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - Frame_Start_Time).count();

		if (Frame_Elapsed_Time < 1000.0f / 60.0f)
		{
			SDL_Delay(int(1000.0f / 60.0f - Frame_Elapsed_Time));
		}

		// Update iteration counter.

		Main_Iteration++;

		// Output framerate every 60 frames.

		if (Main_Iteration % 60 == 0)
		{
			std::cout << "Framerate (frame: " << Main_Iteration << "): " << std::floor(1000.0 / Frame_Elapsed_Time) << std::endl;
		}
	}

	// Destroy the world.

	Destroy_World(The_World);

	// Destroy the chunks.

	for (int i = 0; i < The_Segmenter->Chunk_Count; i++)
	{
		if (The_Segmenter->The_Chunks[i]->Live)
		{
			The_Segmenter->The_Chunks[i]->Destroy();
		}
	}

	// Exit cleanly.

	exit(0);
}