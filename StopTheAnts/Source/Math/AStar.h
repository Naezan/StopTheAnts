#pragma once

#include "../Util/Node.h"

namespace AStar
{
	static void InitNodes(std::vector<std::vector<std::shared_ptr<Node>>>& nodes)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
			for (int y = 0; y < BOARD_HEIGHT; y++)
			{
				nodes[y][x]->position.x = x; // ...because we give each node its own coordinates
				nodes[y][x]->position.y = y; // ...because we give each node its own coordinates
				nodes[y][x]->bObstacle = false;
				nodes[y][x]->parent.reset();
				nodes[y][x]->bVisited = false;
			}

		// Create connections - in this case nodes are on a regular grid
		for (int x = 0; x < BOARD_WIDTH; x++)
			for (int y = 0; y < BOARD_HEIGHT; y++)
			{
				if (y > 0)
					nodes[y][x]->vecNeighbours.push_back(nodes[y - 1][x]);
				if (y < BOARD_HEIGHT - 1)
					nodes[y][x]->vecNeighbours.push_back(nodes[y + 1][x]);
				if (x > 0)
					nodes[y][x]->vecNeighbours.push_back(nodes[y][x - 1]);
				if (x < BOARD_WIDTH - 1)
					nodes[y][x]->vecNeighbours.push_back(nodes[y][x + 1]);

				// We can also connect diagonally
				/*if (y>0 && x>0)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x - 1)]);
				if (y<nMapHeight-1 && x>0)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x - 1)]);
				if (y>0 && x<nMapWidth-1)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 1)]);
				if (y<nMapHeight - 1 && x<nMapWidth-1)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 1)]);
				*/
			}
	}

	static void FindPath(std::vector<std::vector<std::shared_ptr<Node>>>& nodes, std::weak_ptr<Node> nodeStart, std::weak_ptr<Node> nodeEnd)
	{
		// Reset Navigation Graph - default all node states
		for (int x = 0; x < BOARD_WIDTH; ++x)
			for (int y = 0; y < BOARD_HEIGHT; ++y)
			{
				nodes[y][x]->bVisited = false;
				nodes[y][x]->fGlobalGoal = INFINITY;
				nodes[y][x]->fLocalGoal = INFINITY;
				nodes[y][x]->parent.reset();	// No parents
			}

		auto heuristic = [](Vec2D s, Vec2D e) -> int {
			Vec2D delta = std::move(Vec2D{ (s.x - e.x * s.x - e.x),  (s.y - e.y) * (s.y - e.y) });
			return static_cast<int>(sqrt(delta.x + delta.y));
		};

		// Setup starting conditions
		std::weak_ptr<Node> nodeCurrent = nodeStart;
		nodeStart.lock()->fLocalGoal = 0.0f;
		nodeStart.lock()->fGlobalGoal = static_cast<float>(heuristic(nodeStart.lock()->position, nodeEnd.lock()->position));

		// Add start node to not tested list - this will ensure it gets tested.
		// As the algorithm progresses, newly discovered nodes get added to this
		// list, and will themselves be tested later
		std::list<std::weak_ptr<Node>> listNotTestedNodes;
		listNotTestedNodes.push_back(nodeStart);

		// if the not tested list contains nodes, there may be better paths
		// which have not yet been explored. However, we will also stop 
		// searching when we reach the target - there may well be better
		// paths but this one will do - it wont be the longest.
		while (!listNotTestedNodes.empty() && nodeCurrent.lock() != nodeEnd.lock())// Find absolutely shortest path // && nodeCurrent != nodeEnd)
		{
			// Sort Untested nodes by global goal, so lowest is first
			listNotTestedNodes.sort([](const std::weak_ptr<Node> lhs, const std::weak_ptr<Node> rhs) { return lhs.lock()->fGlobalGoal < rhs.lock()->fGlobalGoal; });

			// Front of listNotTestedNodes is potentially the lowest distance node. Our
			// list may also contain nodes that have been visited, so ditch these...
			while (!listNotTestedNodes.empty() && listNotTestedNodes.front().lock()->bVisited)
				listNotTestedNodes.pop_front();

			// ...or abort because there are no valid nodes left to test
			if (listNotTestedNodes.empty())
				break;

			nodeCurrent = listNotTestedNodes.front();
			nodeCurrent.lock()->bVisited = true; // We only explore a node once


			// Check each of this node's neighbours...
			for (auto nodeNeighbour : nodeCurrent.lock()->vecNeighbours)
			{
				// ... and only if the neighbour is not visited and is 
				// not an obstacle, add it to NotTested List
				if (!nodeNeighbour.lock()->bVisited && nodeNeighbour.lock()->bObstacle == 0)
					listNotTestedNodes.push_back(nodeNeighbour);

				// Calculate the neighbours potential lowest parent distance
				float fPossiblyLowerGoal = nodeCurrent.lock()->fLocalGoal + heuristic(nodeCurrent.lock()->position, nodeNeighbour.lock()->position);

				// If choosing to path through this node is a lower distance than what 
				// the neighbour currently has set, update the neighbour to use this node
				// as the path source, and set its distance scores as necessary
				if (fPossiblyLowerGoal < nodeNeighbour.lock()->fLocalGoal)
				{
					nodeNeighbour.lock()->parent = nodeCurrent;
					nodeNeighbour.lock()->fLocalGoal = fPossiblyLowerGoal;

					// The best path length to the neighbour being tested has changed, so
					// update the neighbour's score. The heuristic is used to globally bias
					// the path algorithm, so it knows if its getting better or worse. At some
					// point the algo will realise this path is worse and abandon it, and then go
					// and search along the next best path.
					nodeNeighbour.lock()->fGlobalGoal = nodeNeighbour.lock()->fLocalGoal + heuristic(nodeNeighbour.lock()->position, nodeEnd.lock()->position);
				}
			}
		}
	}


	//TODO 노드를 이어주는 라인을 디버그
	/*if (nodeEnd != nullptr)
	{
		Node* p = nodeEnd;
		while (p->parent != nullptr)
		{
			DrawLine(p->x * nNodeSize + nNodeSize / 2, p->y * nNodeSize + nNodeSize / 2,
				p->parent->x * nNodeSize + nNodeSize / 2, p->parent->y * nNodeSize + nNodeSize / 2, PIXEL_SOLID, FG_YELLOW);

			p = p->parent;
		}
	}*/
}