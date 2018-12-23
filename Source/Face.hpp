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

