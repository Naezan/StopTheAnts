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
		//1������, ��ȣ����
		int dx = (width >= 0) ? 1 : -1;
		int dy = (height > 0) ? 1 : -1;

		//�Ǻ��Ŀ�
		int fw = dx * width;
		int fh = dy * height;

		//�Ǻ��� �ʱⰪ
		int f = isGradularSlope ? fh * 2 - fw : 2 * fw - fh;
		//(x + 1, y)
		int df1 = isGradularSlope ? 2 * fh : 2 * fw;
		//(x + 1, y + 1)
		int df2 = isGradularSlope ? 2 * (fh - fw) : 2 * (fw - fh);

		//��� ����
		if (isGradularSlope)
		{
			for (x = start.x + dx; x < end.x; x += dx)
			{
				//���ǰ˻�
				if (f < 0)
				{
					f += df1;
				}
				else
				{
					y += dy;
					f += df2;
				}
				//���࿡ �����ġ�� obstacle�̸� return false;
				if(nodes[y][x]->bObstacle)
					return false;

				//�����ġ �ϳ��� �߰�
				outPos.push_back(Vec2D{x, y});
			}
			return true;
		}
		//��� ����
		else
		{
			for (y = start.y + dy; y < end.y; y += dy)
			{
				//���ǰ˻�
				if (f < 0)
				{
					f += df1;
				}
				else
				{
					x += dx;
					f += df2;
				}
				//���࿡ �����ġ�� obstacle�̸� return false;
				if (nodes[y][x]->bObstacle)
					return false;

				//�����ġ �ϳ��� �߰�
				outPos.push_back(Vec2D{ x, y });
			}
			return true;
		}
	}
}