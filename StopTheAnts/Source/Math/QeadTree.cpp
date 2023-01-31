#include "../pch.h"
#include "QeadTree.h"
#include "../Game.h"

void QuadTree::QuadTemplate::QuadSplit(std::shared_ptr<QuadTree::QuadNode> quadnode, std::array<std::array<QuadType, BOARD_WIDTH>, BOARD_HEIGHT>& board, int prevx, int prevy, int depth)
{
	int prev = board[prevy][prevx];
	bool isSplit = false;
	for (int y = 0; y < prevy + (BOARD_HEIGHT >> depth); ++y)
	{
		for (int x = 0; x < prevx + (BOARD_WIDTH >> depth); ++x)
		{
			if (x >= BOARD_WIDTH || y >= BOARD_HEIGHT)
				continue;

			if (prev != board[y][x])
			{
				isSplit = true;
				break;
			}
		}
		if (isSplit)
			break;
	}

	if (isSplit)
	{
		++depth;
		//현재 노드 쪼개기
		quadnode->childQuad[0] = std::make_shared<QuadNode>(depth, QuadRect{ prevx, prevy, BOARD_WIDTH >> depth, BOARD_HEIGHT >> depth });
		quadnode->childQuad[1] = std::make_shared<QuadNode>(depth, QuadRect{ prevx + (BOARD_WIDTH >> depth), prevy, BOARD_WIDTH >> depth, BOARD_HEIGHT >> depth });
		quadnode->childQuad[2] = std::make_shared<QuadNode>(depth, QuadRect{ prevx, prevy + (BOARD_HEIGHT >> depth), BOARD_WIDTH >> depth, BOARD_HEIGHT >> depth });
		quadnode->childQuad[3] = std::make_shared<QuadNode>(depth, QuadRect{ prevx + (BOARD_WIDTH >> depth), prevy + (BOARD_HEIGHT >> depth), BOARD_WIDTH >> depth, BOARD_HEIGHT >> depth });

		/*
		0 --- 1
		|     |
		2 --- 3
		*/
		QuadSplit(quadnode->childQuad[0], board, prevx, prevy, depth);
		QuadSplit(quadnode->childQuad[1], board, prevx + (BOARD_WIDTH >> depth), prevy, depth);
		QuadSplit(quadnode->childQuad[2], board, prevx, prevy + (BOARD_HEIGHT >> depth), depth);
		QuadSplit(quadnode->childQuad[3], board, prevx + (BOARD_WIDTH >> depth), prevy + (BOARD_HEIGHT >> depth), depth);
	}
}

std::weak_ptr<Node> QuadTree::QuadTemplate::FindNearestLeafNode(std::weak_ptr<QuadTree::QuadNode> anthouse, std::vector<std::vector<std::shared_ptr<struct Node>>>& nodes)
{
	std::weak_ptr<QuadTree::QuadNode> leafNode;
	float distance = INFINITY;

	//맨처음엔 부모노드부터 찾으면서 depth가 1일때까지 Leaf가 있는지 검사한다.
	//없으면 맨 위 루트노드부터 하나씩 검사해준다.
	auto& checkQuad = anthouse.lock()->parentQuad;
	while (true)
	{
		bool hasLeaf = false;

		for (auto& quad : checkQuad->childQuad)
		{
			//거리가 더 작은 리프 노드를 저장해줍니다.
			if (quad->isLeaf)
			{
				float tmpdist = Math::Distance(anthouse.lock()->quadRect.x, anthouse.lock()->quadRect.y, quad->quadRect.x, quad->quadRect.y);
				if (distance > tmpdist)
				{
					distance = tmpdist;
					leafNode = quad;
				}
				hasLeaf = true;
			}
		}

		//리프가 있다면 break로 빠져나온다.
		if (hasLeaf)
		{
			break;
		}
		//리프가 없다면 그 윗부모를 검사한다.
		else
		{
			//만약에 현재 노드의 depth가 1이라면 검사를 종료하고 전체 검사를 시작한다.
			if (checkQuad->depth == 1)
				break;

			checkQuad = checkQuad->parentQuad;
		}
	}

	//루트쿼드를 찾는다.
	auto& rootQuad = anthouse.lock()->parentQuad;
	while (rootQuad->depth != 0)
	{
		rootQuad = rootQuad->parentQuad;
	}

	//루트노드부터 하나씩 검사를 한다.
	//큐를 만들고 리프가 있으면 자식노드를 하나씩 추가해준다.
	std::queue< std::weak_ptr<QuadTree::QuadNode>> QuadQue;
	QuadQue.push(rootQuad);

	while (!QuadQue.empty())
	{
		auto frontQuad = QuadQue.front();
		QuadQue.pop();
		for (auto& quad : frontQuad.lock()->childQuad)
		{
			//거리가 더 작은 리프 노드를 저장해줍니다.
			if (quad->isLeaf)
			{
				float tmpdist = Math::Distance(anthouse.lock()->quadRect.x, anthouse.lock()->quadRect.y, quad->quadRect.x, quad->quadRect.y);
				if (distance > tmpdist)
				{
					distance = tmpdist;
					leafNode = quad;
				}

				//큐에 추가해준다.
				QuadQue.push(quad);
			}
		}
	}

	//LeafNode를 Node로 변환해줘야한다.
	if (!leafNode.expired())
	{
		return nodes[leafNode.lock()->quadRect.y][leafNode.lock()->quadRect.x];
	}
	return std::weak_ptr<Node>();
}
