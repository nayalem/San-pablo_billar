#pragma once

#include "Point2D.h"

namespace BillarSFML
{
	struct Line
	{
		punto2D start, end;

		enum direct
		{
			RIGHT = 0,
			RIGHT_UP,
			UP,
			LEFT_UP,
			LEFT,
			LEFT_DOWN,
			DOWN,
			RIGHT_DOWN
		} dir;

		Line() : start(), end() {}
	};
}