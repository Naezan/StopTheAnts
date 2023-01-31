#pragma once

#include "Math.h"

struct Node
{
	std::weak_ptr<Node> parent;//노드부모로 나중에 경로를 찾을때 사용됨
	Vec2D position;
	bool bObstacle = false;//노드가 막혀있는지 여부
	bool bVisited = false;//방문여부
	float fGlobalGoal;//예상되는 최종거리
	float fLocalGoal;//현재까지 이동한 거리
	std::vector< std::weak_ptr<Node>> vecNeighbours;//8방향의 노드들
};