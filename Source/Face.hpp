#include <vector>
#include <ostream>

// Face structure.

struct Face_Info
{
	float Top;

	float Bottom;

	float Left;

	float Right;

	float Front;

	float Back;

	Face_Info(float _Top, float _Bottom, float _Left, float _Right, float _Front, float _Back)
	{
		Top = _Top;

		Bottom = _Bottom;

		Left = _Left;

		Right = _Right;

		Front = _Front;

		Back = _Back;
	}
};

// Face_Info* where all faces are the same.

Face_Info* Make_Face_Info_All(float All)
{
	Face_Info* New_Face_Info = new Face_Info(All, All, All, All, All, All);

	return New_Face_Info;
}

// Face_Info* where the Top and Bottom are the same, and all the other faces are the same.

Face_Info* Make_Face_Info_Cap(float Cap, float Side)
{
	Face_Info* New_Face_Info = new Face_Info(Cap, Cap, Side, Side, Side, Side);

	return New_Face_Info;
}

// Face_Info* where the Top and Bottom are the different, and all the other faces are the same.

Face_Info* Make_Face_Info_Caps(float Top, float Bottom, float Side)
{
	Face_Info* New_Face_Info = new Face_Info(Top, Bottom, Side, Side, Side, Side);

	return New_Face_Info;
}

// Lookup Block_ID in Block_Face_Info to get the Face_Info* of that Block_ID.

std::vector<Face_Info*> Block_Face_Info;

void Make_Block_Face_Info()
{
	Block_Face_Info.clear();

	Block_Face_Info = 
	{
		Make_Face_Info_All(-1.0f),

		Make_Face_Info_All(Texture_Name_To_Layer["bedrock"]),

		Make_Face_Info_All(Texture_Name_To_Layer["water_0"]),

		Make_Face_Info_All(Texture_Name_To_Layer["stone"]),

		Make_Face_Info_All(Texture_Name_To_Layer["cobblestone"]),

		Make_Face_Info_All(Texture_Name_To_Layer["dirt"]),

		Make_Face_Info_Caps(Texture_Name_To_Layer["grass_top"], Texture_Name_To_Layer["dirt"], Texture_Name_To_Layer["grass_side"]),

		Make_Face_Info_Caps(Texture_Name_To_Layer["mycelium_top"], Texture_Name_To_Layer["dirt"], Texture_Name_To_Layer["mycelium_side"]),

		Make_Face_Info_All(Texture_Name_To_Layer["sand"]),

		Make_Face_Info_All(Texture_Name_To_Layer["gravel"]),

		Make_Face_Info_All(Texture_Name_To_Layer["brick"]),

		Make_Face_Info_Cap(Texture_Name_To_Layer["oak_planks"], Texture_Name_To_Layer["bookshelf"]),

		Make_Face_Info_Cap(Texture_Name_To_Layer["oak_log_top"], Texture_Name_To_Layer["oak_log_side"]),

		Make_Face_Info_All(Texture_Name_To_Layer["oak_planks"]),

		Make_Face_Info_All(Texture_Name_To_Layer["oak_leaves"]),

		Make_Face_Info_Cap(Texture_Name_To_Layer["birch_log_top"], Texture_Name_To_Layer["birch_log_side"]),

		Make_Face_Info_All(Texture_Name_To_Layer["birch_planks"]),

		Make_Face_Info_All(Texture_Name_To_Layer["birch_leaves"]),

		Make_Face_Info_All(Texture_Name_To_Layer["mushroom_block_red"]),

		Make_Face_Info_All(Texture_Name_To_Layer["mushroom_block_brown"]),

		Make_Face_Info_Cap(Texture_Name_To_Layer["mushroom_block_stem_top"], Texture_Name_To_Layer["mushroom_block_stem_side"]),

		Make_Face_Info_All(Texture_Name_To_Layer["coal_ore"]),

		Make_Face_Info_All(Texture_Name_To_Layer["iron_ore"]),

		Make_Face_Info_All(Texture_Name_To_Layer["gold_ore"]),

		Make_Face_Info_All(Texture_Name_To_Layer["diamond_ore"]),

		Make_Face_Info_All(Texture_Name_To_Layer["emerald_ore"]),

		Make_Face_Info_All(Texture_Name_To_Layer["wool_colored_white"]),

		Make_Face_Info_All(Texture_Name_To_Layer["wool_colored_orange"]),

		Make_Face_Info_All(Texture_Name_To_Layer["wool_colored_magenta"]),

		Make_Face_Info_All(Texture_Name_To_Layer["wool_colored_light_blue"]),

		Make_Face_Info_All(Texture_Name_To_Layer["wool_colored_yellow"]),

		Make_Face_Info_All(Texture_Name_To_Layer["wool_colored_lime"]),

		Make_Face_Info_All(Texture_Name_To_Layer["wool_colored_pink"]),

		Make_Face_Info_All(Texture_Name_To_Layer["wool_colored_gray"]),

		Make_Face_Info_All(Texture_Name_To_Layer["wool_colored_silver"]),

		Make_Face_Info_All(Texture_Name_To_Layer["wool_colored_cyan"]),

		Make_Face_Info_All(Texture_Name_To_Layer["wool_colored_purple"]),

		Make_Face_Info_All(Texture_Name_To_Layer["wool_colored_blue"]),

		Make_Face_Info_All(Texture_Name_To_Layer["wool_colored_brown"]),

		Make_Face_Info_All(Texture_Name_To_Layer["wool_colored_green"]),

		Make_Face_Info_All(Texture_Name_To_Layer["wool_colored_red"]),

		Make_Face_Info_All(Texture_Name_To_Layer["wool_colored_black"])
	};
}

// Debugs Block_Face_Info.

void Debug_Block_Face_Info(std::ostream& Out)
{
	for (int i = 1; i < Block_Face_Info.size(); i++)
	{
		Out << Block_ID_To_Block_Name[i] << ": " << std::endl << '\t' <<

			"Top: " << All_Block_Texture_Paths[int(Block_Face_Info[i]->Top)] << ", " << std::endl << '\t' <<

			"Bottom: " << All_Block_Texture_Paths[int(Block_Face_Info[i]->Bottom)] << ", " << std::endl << '\t' <<

			"Left: " << All_Block_Texture_Paths[int(Block_Face_Info[i]->Left)] << ", " << std::endl << '\t' <<

			"Right: " << All_Block_Texture_Paths[int(Block_Face_Info[i]->Right)] << ", " << std::endl << '\t' <<

			"Front: " << All_Block_Texture_Paths[int(Block_Face_Info[i]->Front)] << ", " << std::endl << '\t' <<

			"Back: " << All_Block_Texture_Paths[int(Block_Face_Info[i]->Back)] << ", " << std::endl;
	}
}