#pragma once

#include "../Util/Node.h"

namespace Bresenham
{
	static bool FindStraightPath(const std::vector<std::vector<std::shared_ptr<Node>>>& nodes, const Vec2D& start, const Vec2D& end, std::deque<Vec2D>& outPos)
	{
		int width = end.x - start.x;
		int height = end.y - start.y;

		bool isGradularSlope = Math::Abs(width) >= Math::Abs(height);

		int x = start.x;
		int y = start.y;
		//1씩증가, 부호여부
		int dx = (width >= 0) ? 1 : -1;
		int dy = (height > 0) ? 1 : -1;

		//판별식용
		int fw = dx * width;
		int fh = dy * height;

		//판별식 초기값
		int f = isGradularSlope ? fh * 2 - fw : 2 * fw - fh;
		//(x + 1, y)
		int df1 = isGradularSlope ? 2 * fh : 2 * fw;
		//(x + 1, y + 1)
		int df2 = isGradularSlope ? 2 * (fh - fw) : 2 * (fw - fh);

		//경사 낮음
		if (isGradularSlope)
		{
			for (x = start.x + dx; x < end.x; x += dx)
			{
				//조건검사
				if (f < 0)
				{
					f += df1;
				}
				else
				{
					y += dy;
					f += df2;
				}
				//만약에 노드위치가 obstacle이면 return false;
				if(nodes[y][x]->bObstacle)
					return false;

				//노드위치 하나씩 추가
				outPos.push_back(Vec2D{x, y});
			}
			return true;
		}
		//경사 높음
		else
		{
			for (y = start.y + dy; y < end.y; y += dy)
			{
				//조건검사
				if (f < 0)
				{
					f += df1;
				}
				else
				{
					x += dx;
					f += df2;
				}
				//만약에 노드위치가 obstacle이면 return false;
				if (nodes[y][x]->bObstacle)
					return false;

				//노드위치 하나씩 추가
				outPos.push_back(Vec2D{ x, y });
			}
			return true;
		}
	}
}