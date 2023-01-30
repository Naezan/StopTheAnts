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
