#include "Vector2.h"

std::ostream& operator<< (std::ostream& out, const Vector2& vec)
{
	out << vec.x << " " << vec.y;
	return out;
}