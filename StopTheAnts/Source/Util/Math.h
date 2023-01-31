#pragma once

struct Vec2D
{
	int x, y;

	bool operator==(const Vec2D& vec2)
	{
		return x == vec2.x && y == vec2.y;
	}

	Vec2D operator+(const Vec2D& r)
	{
		return { x + r.x, y + r.y };
	}
};

namespace Math
{
	template<class T>
	static constexpr T Abs(const T& A)
	{
		return (A >= (T)0) ? A : -A;
	}

	static constexpr float Distance(const Vec2D& l, const Vec2D& r)
	{
		return (l.x - r.x) * (l.x - r.x) + (l.y - r.y) * (l.y - r.y);
	}

	static constexpr float Distance(int x1, int y1, int x2, int y2)
	{
		return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
	}
}