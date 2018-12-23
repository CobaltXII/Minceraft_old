#include <iostream>
#include <ostream>
#include <string>
#include <map>

// Generates a map that maps block texture names (exluding .png and ../Assets/Blocks/) to layer
// indices in a 2D texture array. Returns the pointer to that 2D texture array.

std::map<std::string, float> Texture_Name_To_Layer;

GLuint Generate_Block_Texture_Array()
{
	GLsizei Expected_Width = 16;
	GLsizei Expected_Height = 16;

	int Expected_Channels = 4;

	GLsizei Layer_Count = All_Block_Texture_Paths.size();

	GLubyte* Block_Texture_Array_Texels = (GLubyte*)malloc(Expected_Width * Expected_Height * Expected_Channels * Layer_Count);

	if (!Block_Texture_Array_Texels)
	{
		std::cout << "Could not allocate space for block texture array." << std::endl;

		exit(4);
	}

	for (int i = 0; i < Layer_Count; i++)
	{
		std::string Full_Path = "../" + All_Block_Texture_Paths[i];

		// Load that texture!

		int Original_W;
		int Original_H;

		int Original_Channels;

		unsigned char* Original_Data = stbi_load(Full_Path.c_str(), &Original_W, &Original_H, &Original_Channels, STBI_rgb_alpha);

		if (!Original_Data)
		{
			std::cout << "Could not load \"" << Full_Path << "\"." << std::endl;

			exit(5);
		}

		if (Original_W != Expected_Width || Original_H != Expected_Height)
		{
			std::cout << "Dimensions of \"" << Full_Path << "\" (" << Original_W << ", " << Original_H << ") do not match expected dimensions (" << Expected_Width << ", " << Expected_Height << ")." << std::endl;
		
			exit(6);
		}

		if (Original_Channels != Expected_Channels)
		{
			std::cout << "Channel count of \"" << Full_Path << "\" (" << Original_Channels << ") does not match expected channel count (" << Expected_Channels << ")." << std::endl;

			exit(7); 
		}

		// Copy to texel array.

		memcpy(Block_Texture_Array_Texels + (i * Expected_Width * Expected_Height * Expected_Channels), Original_Data, Expected_Width * Expected_Height * Expected_Channels);

		// Free data.

		stbi_image_free(Original_Data);

		// Add to map.

		std::string Short_Path = std::string(All_Block_Texture_Paths[i]).erase(0, 7);

		Short_Path = Short_Path.substr(0, Short_Path.length() - 4);

		Texture_Name_To_Layer.emplace(Short_Path, float(i));
	}

	// Generate texture array.

	GLuint Block_Texture_Array = 0;

	glGenTextures(1, &Block_Texture_Array);

	glBindTexture(GL_TEXTURE_2D_ARRAY, Block_Texture_Array);

	// Allocate storage for the 2D texture array. Thanks a lot to 
	// https://stackoverflow.com/users/3530129/reto-koradi

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, Expected_Width, Expected_Height, Layer_Count, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// Upload texel data.

	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, Expected_Width, Expected_Height, Layer_Count, GL_RGBA, GL_UNSIGNED_BYTE, Block_Texture_Array_Texels);

	// Set up texture parameters.

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Unbind and return.

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return Block_Texture_Array;
}

// Debugger for Texture_Name_To_Layer.

void Debug_Texture_Name_To_Layer(std::ostream& Out)
{
	for (int i = 0; i < All_Block_Texture_Paths.size(); i++)
	{
		std::string Short_Path = std::string(All_Block_Texture_Paths[i]).erase(0, 7);

		Short_Path = Short_Path.substr(0, Short_Path.length() - 4);

		std::cout << "Key: " << Short_Path << ", Value: " << Texture_Name_To_Layer.at(Short_Path) << " (" << i << ")." << std::endl; 
	}
}
