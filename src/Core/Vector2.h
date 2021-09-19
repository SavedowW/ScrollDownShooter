#ifndef VECTOR2_H_
#define VECTOR2_H_
#include <cmath>
#include <iostream>

const float PI = 3.1415;

struct Vector2
{
	float x, y;
	inline Vector2(float nx = 0, float ny = 0)
	{
		x = nx;
		y = ny;
	}

	inline Vector2 operator+(const Vector2& rhs) const
	{
		return { x + rhs.x, y + rhs.y };
	}

	inline Vector2 operator-(const Vector2& rhs) const
	{
		return { x - rhs.x, y - rhs.y };
	}

	inline Vector2 operator*(const float& num) const
	{
		return { x * num, y * num };
	}

	inline Vector2 operator/(const float& num) const
	{
		return { x / num, y / num };
	}

	inline void operator+=(const Vector2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}

	inline void operator-=(const Vector2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
	}

	inline void operator*=(const float& num)
	{
		x *= num;
		y *= num;
	}

	inline void operator/=(const float& num)
	{
		x /= num;
		y /= num;
	}

	inline float getLen() const
	{
		return sqrt(x * x + y * y);
	}

	inline float getSqLen() const
	{
		return x * x + y * y;
	}

	inline void normalise()
	{
		float l = sqrt(x * x + y * y);
		if (l)
		{
			x = x / l;
			y = y / l;
		}
	}

	inline Vector2 normalised() const
	{
		float l = sqrt(x * x + y * y);
		return Vector2(x / l, y / l);
	}
};

std::ostream& operator<< (std::ostream& out, const Vector2& vec);

//Only rectangle hitbox
struct Hitbox
{
	float x, y, w, h;
	inline Hitbox(float nx = 0, float ny = 0, float nw = 0, float nh = 0)
	{
		x = nx;
		y = ny;
		w = nw;
		h = nh;
	}
	inline Hitbox operator+(const Vector2& rhs) const
	{
		return { x + rhs.x, y + rhs.y, w, h };
	}
	inline int isCollideWith_x(const Hitbox& hbox) const
	{
		if (x <= hbox.x && x + w > hbox.x)
			return 1;
		if (x + w >= hbox.x + hbox.w && x < hbox.x + hbox.w)
			return 2;
		if (x >= hbox.x && x + w <= hbox.x + hbox.w)
			return 4;
		else
			return 0;
	}
	inline int isCollideWith_y(const Hitbox& hbox) const
	{
		if (y <= hbox.y && y + h > hbox.y)
			return 1;
		if (y + h >= hbox.y + hbox.h && y < hbox.y + hbox.h)
			return 2;
		if (y >= hbox.y && y + h <= hbox.y + hbox.h)
			return 4;
		else
			return 0;
	}
	inline bool isCollideWith(const Hitbox& hbox) const
	{
		return isCollideWith_x(hbox) && isCollideWith_y(hbox);
	}
	inline Vector2 getPos() const
	{
		return Vector2(x, y);
	}
	inline Vector2 getSize() const
	{
		return Vector2(w, h);
	}
	inline Vector2 getCenter() const
	{
		return Vector2(x + w / 2, y + h / 2);
	}
	
};

#endif