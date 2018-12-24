// Generate a hotbar sprite (located correctly).

Final_Sprite* Make_Hotbar_Sprite(int X_Res, int Y_Res, int Factor)
{
	// Scale the original size by Factor.

	int Sprite_X_Res = Interface_Textures["hotbar"]->X_Res * Factor;
	int Sprite_Y_Res = Interface_Textures["hotbar"]->Y_Res * Factor;

	// Centered around the Y axis.

	int Origin_X = X_Res / 2 - Sprite_X_Res / 2;

	// Just above the bottom of the screen.

	int Origin_Y = Y_Res - Sprite_Y_Res;

	// Generate the sprite.

	Final_Sprite* My_Final_Sprite = Generate_Rectangle_Sprite
	(
		Origin_X, 
		Origin_Y, 

		Sprite_X_Res + Origin_X, 
		Sprite_Y_Res + Origin_Y, 

		X_Res, 
		Y_Res, 

		Interface_Textures["hotbar"]->Storage
	);

	return My_Final_Sprite;
}

// Draw a hotbar with a selection border.

void Draw_Hotbar(int X_Res, int Y_Res, int Hi_X_Res, int Hi_Y_Res, int Selection, GLuint Block_Texture_Array, int Factor)
{
	Draw_Sprite(Make_Hotbar_Sprite(X_Res, Y_Res, Factor));

	int Selection_X_Res = Interface_Textures["hotbar_selection"]->X_Res * Factor;
	int Selection_Y_Res = Interface_Textures["hotbar_selection"]->Y_Res * Factor;

	// The X and Y position of the top left corner of the hotbar selection is based on the 
	// position of the top left corner of the hotbar, offset by 4 in the negative X and Y axes.
	// The X position is then offset further by the Selection constant multiplied by a magic 
	// number.

	int Selection_X = X_Res / 2 - Interface_Textures["hotbar"]->X_Res * Factor / 2 - Factor;

	Selection_X += Selection * Factor * 20;

	int Selection_Y = Y_Res - (Interface_Textures["hotbar"]->Y_Res * Factor) - Factor;

	Final_Sprite* Selection_Sprite = Generate_Rectangle_Sprite
	(
		Selection_X, 
		Selection_Y, 

		Selection_X_Res + Selection_X, 
		Selection_Y_Res + Selection_Y, 

		X_Res, 
		Y_Res, 

		Interface_Textures["hotbar_selection"]->Storage
	);

	Draw_Sprite(Selection_Sprite);

	// Draw the items available in the hotbar.

	int Hotbar_X_Res = Interface_Textures["hotbar"]->X_Res * Factor;
	int Hotbar_Y_Res = Interface_Textures["hotbar"]->Y_Res * Factor;

	int Hotbar_X = X_Res / 2 - Hotbar_X_Res / 2;

	int Hotbar_Y = Y_Res - Hotbar_Y_Res;

	for (int i = 0; i < 9; i++)
	{
		Render_Isometric(Hotbar_X + (20 * i + 3) * Factor, Hotbar_Y + 3 * Factor, 16 * Factor, 16 * Factor, X_Res, Y_Res, Hi_X_Res, Hi_Y_Res, Block_ID(id_water + i), Block_Texture_Array);
	}
}