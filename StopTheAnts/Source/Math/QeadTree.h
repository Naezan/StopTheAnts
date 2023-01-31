#pragma once

#include "../Util/Node.h"

namespace QuadTree
{
	struct QuadRect
	{
		int x, y, width, height;
	};

	struct QuadNode
	{
		QuadNode(int _depth = 0, QuadRect rect = { 0,0,0,0 }) : depth(_depth), quadRect(rect)
		{

		}

		int depth;//기본0 하나늘때마다 + 1
		bool isLeaf = false;
		QuadTree::QuadRect quadRect;
		std::shared_ptr<QuadTree::QuadNode> parentQuad;
		std::shared_ptr<QuadTree::QuadNode> childQuad[4];
	};

	class QuadTemplate
	{
		static void QuadSplit(std::shared_ptr<QuadTree::QuadNode> quadnode, std::array<std::array<enum QuadType, BOARD_WIDTH>, BOARD_HEIGHT>& board, int prevx, int prevy, int depth);

		//단점 단순히 개미집을 vector로 만든것보다 복잡하다 가독성이 떨어진다. 내 프로젝트에선 분류 갯수(나뭇잎의 개수)가 많지 않다.
		//그렇기 때문에 굳이 QuadTree를 사용할필요가 없다.
		static std::weak_ptr<Node> FindNearestLeafNode(std::weak_ptr<QuadTree::QuadNode> anthouse, std::vector<std::vector<std::shared_ptr<struct Node>>>& nodes);
	};
}