#include "NavMesh.h"
#include "Application2D.h"

//destructor, deletes all path nodes and path connections
NavMesh::~NavMesh()
{
	//iterate through all vertices
	for (size_t i = 0; i < data.vertices.size(); i++)
	{
		Vertex<NavNode*, NavConnection*>* vertex = data.vertices[i];

		//iterate through all edges in the vertex
		for (size_t j = 0; j < vertex->edges.size(); j++)
		{
			Edge<NavNode*, NavConnection*> edge = vertex->edges[j];

			//delete the path connection
			delete edge.value;
		}

		//delete the path node
		delete vertex->data;
	}
}

//creates a 2D grid set to specific preferences
void NavMesh::createGrid(unsigned int sizeX, unsigned int sizeY, Vector2 origin, Vector2 spacing)
{
	//2D container used to aid in connecting vertices
	std::vector<std::vector<Vertex<NavNode*, NavConnection*>*>> grid;

	//iterate through the rows
	for (size_t i = 0; i < sizeY; i++)
	{
		//a single row in the grid
		std::vector<Vertex<NavNode*, NavConnection*>*> row;

		//iterate through the columns
		for (size_t j = 0; j < sizeX; j++)
		{
			NavNode* pathNode = new NavNode();

			//offset the node from the origin based on it's position in the grid
			pathNode->position = origin + Vector2(spacing.x * j, spacing.y * i);

			//add the vertex
			data.addVertex(pathNode);

			//get the vertex that was just created and place it in the grid structure
			row.push_back(data.vertices[i * sizeX + j]);
		}

		//add the row to the grid
		grid.push_back(row);
	}

	//iterate through the nodes, connecting them to the adjacent nodes
	for (size_t i = 0; i < sizeY; i++)
	{
		//iterate through the columns
		for (size_t j = 0; j < sizeX; j++)
		{
			//iterate through the adjacent rows
			for (int y = -1; y < 2; y++)
			{
				//iterate through the adjacent columns
				for (int x = -1; x < 2; x++)
				{
					//check that the iterator is going through an adjacent and not the same node
					if (y == 0 && x == 0)
					{
						continue;
					}

					//check that the iterator is still in the grid
					if (i + y >= 0 && i + y < sizeY && j + x >= 0 && j + x < sizeX)
					{
						//get the vertices involved in the connection
						Vertex<NavNode*, NavConnection*>* start = grid[i][j];
						Vertex<NavNode*, NavConnection*>* end = grid[i + y][j + x];

						data.createConnection(start, end);

						//get the connection that was just created
						Edge<NavNode*, NavConnection*>* conn = &start->edges[start->edges.size() - 1];

						conn->value = new NavConnection();

						//the distance between the two nodes is the cost
						conn->value->cost = (end->data->position - start->data->position).magnitude();
					}
				}
			}
		}
	}

}

//resets all nodes that A* manipulated during it's search
void NavMesh::resetSearchedNodes(std::vector<Vertex<NavNode*, NavConnection*>*> open, std::vector<Vertex<NavNode*, NavConnection*>*> closed)
{
	//iterate through all open nodes, resetting them all
	for (size_t i = 0; i < open.size(); i++)
	{
		//temporary value for readability and performance
		NavNode* pn = open[i]->data;

		//reset the values
		pn->gScore = INFINITY;
		pn->inOpen = false;
		pn->visited = false;
		pn->previous = nullptr;
	}

	//iterate through all closed nodes, resetting them all
	for (size_t i = 0; i < closed.size(); i++)
	{
		//temporary value for readability and performance
		NavNode* pn = closed[i]->data;

		//reset the values
		pn->gScore = INFINITY;
		pn->inOpen = false;
		pn->visited = false;
		pn->previous = nullptr;
	}
}

//calculates the shortest path from the starting node to the ending node using the A* pathfinding algorithm
std::vector<Vector2> NavMesh::calculateAStarPath(Vertex<NavNode*, NavConnection*>* start, Vertex<NavNode*, NavConnection*>* end, float E)
{
	//the open list
	std::vector<Vertex<NavNode*, NavConnection*>*> open;

	//the closed list, used to reset the nodes
	std::vector<Vertex<NavNode*, NavConnection*>*> closed;

	start->data->visited = true;
	start->data->gScore = 0.0f;

	open.push_back(start);

	//while the open list still contains 
	while (open.size() > 0)
	{

		//remember the node with the lowest 'F' score in the open list
		Vertex<NavNode*, NavConnection*>* best = open[0];
		size_t bestI = 0;

		//iterate through all nodes, finding the node with the smallest 'F' score
		for (size_t i = 1; i < open.size(); i++)
		{
			//store in a temp variable, saves performance and increases readability
			Vertex<NavNode*, NavConnection*>* node = open[i];

			//calculate the 'F' score of both nodes to compare them
			float bestF = best->data->gScore + heursticFunction(best->data->position, end->data->position) * E;
			float nodeF = node->data->gScore + heursticFunction(node->data->position, end->data->position) * E;

			//if the best 'F' score is higher than the current node's 'F' score
			if (bestF > nodeF)
			{
				//the current node is now the best node
				best = node;
				bestI = i;
			}
		}

		//if the best node in the open list is the end, the goal has been found
		if (best == end)
		{
			//list of vectors that make up the path
			std::vector<Vector2> pointList;

			NavNode* current = best->data;

			//backtrack using the previous nodes until the start is reached
			while (current != start->data)
			{
				//add the node's position to the front of the list
				pointList.insert(pointList.begin(), current->position);

				//go to the previous node
				current = current->previous;
			}

			//add the starting node's position
			pointList.insert(pointList.begin(), current->position);

			//reset the lists of open and closed nodes
			resetSearchedNodes(open, closed);

			return pointList;
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
			Edge<NavNode*, NavConnection*> edge = best->edges[i];

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
			if (test_gScore >= edge.end->data->gScore)
			{
				continue;
			}

			//set the previous best path
			edge.end->data->previous = best->data;
			
			//replace the 'G' score with the improved 'G' score
			edge.end->data->gScore = test_gScore;
		}

		
	}

	//the algorithm failed to find the ending point, and all nodes connected to the starting point were searched

	//reset the lists of open and closed nodes
	resetSearchedNodes(open, closed);

	return std::vector<Vector2>();

}

//draws the graph structure
void NavMesh::drawMesh(float nodeRadius, float connectionThickness, float r, float g, float b)
{
	//set the desired colour to draw the graph with
	appPtr->m_renderer2D->setRenderColour(r, g, b);

	//iterate through all vertices, drawing a circle for each one
	for (size_t i = 0; i < data.vertices.size(); i++)
	{
		Vertex<NavNode*, NavConnection*>* vertex = data.vertices[i];

		//iterate through all edges in the vertex, drawing a line for each one
		for (size_t j = 0; j < vertex->edges.size(); j++)
		{
			Edge<NavNode*, NavConnection*> edge = vertex->edges[j];

			//draw the edge
			appPtr->m_renderer2D->drawLine(edge.start->data->position.x, edge.start->data->position.y, edge.end->data->position.x, edge.end->data->position.y, connectionThickness);
		}

		//draw the node
		appPtr->m_renderer2D->drawCircle(vertex->data->position.x, vertex->data->position.y, nodeRadius);
	}

	//reset the colour
	appPtr->m_renderer2D->setRenderColour(1.0f, 1.0f, 1.0f);
}
