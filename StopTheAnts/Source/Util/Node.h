#pragma once

#include "Math.h"

struct Node
{
	std::weak_ptr<Node> parent;//���θ�� ���߿� ��θ� ã���� ����
	Vec2D position;
	bool bObstacle = false;//��尡 �����ִ��� ����
	bool bVisited = false;//�湮����
	float fGlobalGoal;//����Ǵ� �����Ÿ�
	float fLocalGoal;//������� �̵��� �Ÿ�
	std::vector< std::weak_ptr<Node>> vecNeighbours;//8������ ����
};