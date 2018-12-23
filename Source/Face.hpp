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
