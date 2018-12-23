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

