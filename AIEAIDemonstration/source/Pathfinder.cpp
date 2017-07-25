#include "Pathfinder.h"

//destructor, deletes all path nodes and path connections
PathfindingMap::~PathfindingMap()
{
}

//calculates the shortest path from the starting node to the ending node using the A* pathfinding algorithm
std::vector<Vector2> PathfindingMap::calculateAStarPath(Vertex<PathNode*, PathConnection*>* start, Vertex<PathNode*, PathConnection*>* end)
{
	//the open list
	std::vector<Vertex<PathNode*, PathConnection*>*> open;

	//the closed list, used to reset the nodes
	std::vector<Vertex<PathNode*, PathConnection*>*> closed;

	start->data->visited = true;
	open.push_back(start);

	//while the open list still contains 
	while (open.size() > 0)
	{

		//remember the node with the lowest 'F' score in the open list
		Vertex<PathNode*, PathConnection*>* best = open[0];
		size_t bestI = 0;

		//iterate through all nodes, finding the node with the smallest 'F' score
		for (size_t i = 1; i < open.size(); i++)
		{
			//store in a temp variable, saves performance and increases readability
			Vertex<PathNode*, PathConnection*>* node = open[i];

			//calculate the 'F' score of both nodes to compare them
			float bestF = best->data->gScore + heursticFunction(best->data->position, end->data->position);
			float nodeF = node->data->gScore + heursticFunction(node->data->position, end->data->position);

			//if the best 'F' score is higher than the current node's 'F' score
			if (bestF > nodeF)
			{
				//the current node is now the best node
				best = node;
			}
		}

		//if the best node in the open list is the end, the goal has been found
		if (best == end)
		{
			//OMG FUCKING AMAZING CAN WE GO BACK NOW?
			//--------------------------------------
			//--------------------------------------
			//--------------------------------------
		}

		//remove the node from the open list
		open.erase(open.begin() + bestI);

		//set the inOpen flag to false
		best->data->inOpen = false;

		//set the visited flag to true
		best->data->visited = true;

		//add the node to the closed list
		closed.push_back(best);

		//iterate through all edges, discovering new nodes
		for (size_t i = 0; i < best->edges.size(); i++)
		{
			//store in a temp variable, saves performance and increases readability
			Edge<PathNode*, PathConnection*> edge = best->edges[i];

			//don't add the node at the end of the connection if it has already been visited
			if (edge.end->data->visited)
			{
				continue;
			}

			//don't add the node if it is already in the open list
			if (!edge.end->data->inOpen)
			{
				edge.end->data->inOpen = true;
				open.push_back(edge.end);
			}

			//the g score that the node would have if it wasn't transversed already
			float test_gScore = best->data->gScore + edge.value->cost;

			//don't do anything if the proposed new g score for the node doesn't improve it
			if (test_gScore >= end->data->gScore)
			{
				continue;
			}

			//set the previous best path
			edge.end->data->previous = best->data;
			
			//replace the 'G' score with the improved 'G' score
			edge.end->data->gScore = test_gScore;
		}

		
	}

	//YOU KNOW YOU'RE FUCKED WHEN EVEN A PATHFINDER 
	//CAN'T GUIDE YOU TO YOUR DESTINATION
	//--------------------------------------
	//--------------------------------------
	//--------------------------------------
}
