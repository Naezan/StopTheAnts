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
		//���� ��� �ɰ���
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

	//��ó���� �θ������ ã���鼭 depth�� 1�϶����� Leaf�� �ִ��� �˻��Ѵ�.
	//������ �� �� ��Ʈ������ �ϳ��� �˻����ش�.
	auto& checkQuad = anthouse.lock()->parentQuad;
	while (true)
	{
		bool hasLeaf = false;

		for (auto& quad : checkQuad->childQuad)
		{
			//�Ÿ��� �� ���� ���� ��带 �������ݴϴ�.
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

		//������ �ִٸ� break�� �������´�.
		if (hasLeaf)
		{
			break;
		}
		//������ ���ٸ� �� ���θ� �˻��Ѵ�.
		else
		{
			//���࿡ ���� ����� depth�� 1�̶�� �˻縦 �����ϰ� ��ü �˻縦 �����Ѵ�.
			if (checkQuad->depth == 1)
				break;

			checkQuad = checkQuad->parentQuad;
		}
	}

	//��Ʈ���带 ã�´�.
	auto& rootQuad = anthouse.lock()->parentQuad;
	while (rootQuad->depth != 0)
	{
		rootQuad = rootQuad->parentQuad;
	}

	//��Ʈ������ �ϳ��� �˻縦 �Ѵ�.
	//ť�� ����� ������ ������ �ڽĳ�带 �ϳ��� �߰����ش�.
	std::queue< std::weak_ptr<QuadTree::QuadNode>> QuadQue;
	QuadQue.push(rootQuad);

	while (!QuadQue.empty())
	{
		auto frontQuad = QuadQue.front();
		QuadQue.pop();
		for (auto& quad : frontQuad.lock()->childQuad)
		{
			//�Ÿ��� �� ���� ���� ��带 �������ݴϴ�.
			if (quad->isLeaf)
			{
				float tmpdist = Math::Distance(anthouse.lock()->quadRect.x, anthouse.lock()->quadRect.y, quad->quadRect.x, quad->quadRect.y);
				if (distance > tmpdist)
				{
					distance = tmpdist;
					leafNode = quad;
				}

				//ť�� �߰����ش�.
				QuadQue.push(quad);
			}
		}
	}

	//LeafNode�� Node�� ��ȯ������Ѵ�.
	if (!leafNode.expired())
	{
		return nodes[leafNode.lock()->quadRect.y][leafNode.lock()->quadRect.x];
	}
	return std::weak_ptr<Node>();
}
