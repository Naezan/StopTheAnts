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

		int depth;//�⺻0 �ϳ��ö����� + 1
		bool isLeaf = false;
		QuadTree::QuadRect quadRect;
		std::shared_ptr<QuadTree::QuadNode> parentQuad;
		std::shared_ptr<QuadTree::QuadNode> childQuad[4];
	};

	class QuadTemplate
	{
		static void QuadSplit(std::shared_ptr<QuadTree::QuadNode> quadnode, std::array<std::array<enum QuadType, BOARD_WIDTH>, BOARD_HEIGHT>& board, int prevx, int prevy, int depth);

		//���� �ܼ��� �������� vector�� ����ͺ��� �����ϴ� �������� ��������. �� ������Ʈ���� �з� ����(�������� ����)�� ���� �ʴ�.
		//�׷��� ������ ���� QuadTree�� ������ʿ䰡 ����.
		static std::weak_ptr<Node> FindNearestLeafNode(std::weak_ptr<QuadTree::QuadNode> anthouse, std::vector<std::vector<std::shared_ptr<struct Node>>>& nodes);
	};
}