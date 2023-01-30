#pragma once

#include "Math.h"

struct Node
{
	Node* parent;//���θ�� ���߿� ��θ� ã���� ����
	Vec2D position;
	bool bObstacle = false;//��尡 �����ִ��� ����
	bool bVisited = false;//�湮����
	float fGlobalGoal;//����Ǵ� �����Ÿ�
	float fLocalGoal;//������� �̵��� �Ÿ�
	std::vector<Node*> vecNeighbours;//8������ ����
};