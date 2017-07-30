#include "NavMesh.h"
#include "Application2D.h"

//collision math
#include "CollisionEngine.h"

//headers for file I/O
#include <iostream>
#include <fstream>

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



//creates graph data that matches the
void NavMesh::createMesh(std::vector<Vector2> vertices, std::vector<int> triangles)
{
	//iterate through the list of Vector2's (vertices), adding a vertex to the graph for each one
	for (size_t i = 0; i < vertices.size(); i++)
	{
		//create a nav node with the correct position
		NavNode* navNode = new NavNode();
		navNode->position = vertices[i];
		navNode->vertexIndex = i;

		data.addVertex(navNode);
	}

	//iterate through the list of triangles, adding three connections for each
	for (size_t i = 0; i < triangles.size(); i += 3)
	{
		//get the three vertices involved in the triangle
		Vertex<NavNode*, NavConnection*>* vert1 = data.vertices[triangles[i] - 1];
		Vertex<NavNode*, NavConnection*>* vert2 = data.vertices[triangles[i+1] - 1];
		Vertex<NavNode*, NavConnection*>* vert3 = data.vertices[triangles[i+2] - 1];

		//six connections are formed, two per vertex
		NavConnection* n12 = new NavConnection();
		n12->cost = (vert2->data->position - vert1->data->position).magnitude();

		NavConnection* n21 = new NavConnection();
		n21->cost = n12->cost;

		NavConnection* n23 = new NavConnection();
		n23->cost = (vert3->data->position - vert2->data->position).magnitude();

		NavConnection* n32 = new NavConnection();
		n32->cost = n23->cost;

		NavConnection* n31 = new NavConnection();
		n31->cost = (vert1->data->position - vert3->data->position).magnitude();

		NavConnection* n13 = new NavConnection();
		n13->cost = n31->cost;

		//connect vertices creates two edges
		data.connectVertices(vert1, vert2);
		data.connectVertices(vert2, vert3);
		data.connectVertices(vert3, vert1);

		//give the recently added edges their connection values
		vert1->edges[vert1->edges.size() - 2].value = n12;
		vert2->edges[vert2->edges.size() - 2].value = n21;
		vert2->edges[vert2->edges.size() - 1].value = n23;
		vert3->edges[vert3->edges.size() - 2].value = n32;
		vert3->edges[vert3->edges.size() - 1].value = n31;
		vert1->edges[vert1->edges.size() - 1].value = n13;

		/*
		* in order to draw the triangle mesh properly using the bootstrap,
		* the points must be in counter-clockwise order otherwise bootstrap
		* will render the triangle facing away from the viewport
		* the code below ensures that the list of vertices are always in the
		* correct order to be drawn to the viewport correctly
		*/

		//sort the vertices based on their y axis
		Vertex<NavNode*, NavConnection*>* topVertex = nullptr;
		Vertex<NavNode*, NavConnection*>* centreVertex = nullptr;
		Vertex<NavNode*, NavConnection*>* bottomVertex = nullptr;

		//test case for the first vertex
		if (vert1->data->position.y < fmaxf(vert2->data->position.y, vert3->data->position.y) && vert1->data->position.y > fminf(vert2->data->position.y, vert3->data->position.y))
		{
			centreVertex = vert1;

			//test if vert2 is the bottom
			if (vert2->data->position.y < centreVertex->data->position.y)
			{
				bottomVertex = vert2;
				topVertex = vert3;
			}
			else
			{
				bottomVertex = vert3;
				topVertex = vert2;
			}
		}
		//test case for the second vertex
		else if (vert2->data->position.y < fmaxf(vert1->data->position.y, vert3->data->position.y) && vert2->data->position.y > fminf(vert1->data->position.y, vert3->data->position.y))
		{
			centreVertex = vert2;

			//test if vert1 is the bottom
			if (vert1->data->position.y < centreVertex->data->position.y)
			{
				bottomVertex = vert1;
				topVertex = vert3;
			}
			else
			{
				bottomVertex = vert3;
				topVertex = vert1;
			}
		}
		//no point testing the third vertex if the first and second aren't the middle vertex
		else
		{
			centreVertex = vert3;

			//test if vert1 is the bottom
			if (vert1->data->position.y < centreVertex->data->position.y)
			{
				bottomVertex = vert1;
				topVertex = vert2;
			}
			else
			{
				bottomVertex = vert2;
				topVertex = vert1;
			}
		}

		triangleData.push_back(centreVertex);

		//which side of the line between the bottom and top vertices is the middle vertex on
		if (COLL_ENGINE->calculateSide(bottomVertex->data->position, topVertex->data->position, centreVertex->data->position) >= 0)
		{ 
			//left/middle
			triangleData.push_back(bottomVertex);
			triangleData.push_back(topVertex);
		}
		else
		{
			//right
			triangleData.push_back(topVertex);
			triangleData.push_back(bottomVertex);
		}
	}
}



//calculates common edges between triangles in the mesh
void NavMesh::calculateSharedEdges()
{

	//iterate through all triangles, comparing them to every other triangle
	for (size_t i = 0; i < triangleData.size(); i += 3)
	{
		for (size_t j = 0; j < triangleData.size(); j += 3)
		{
			//don't compare a triangle to itself
			if (i == j)
			{
				continue;
			}

			//secondary iteration, itrerates accross the sides in the current triangle
			for (size_t t1 = 0; t1 < 3; t1++)
			{
				for (size_t t2 = 0; t2 < 3; t2++)
				{
					//index of the last vertex in the sides
					size_t t12 = (t1 + 1) % 3;
					size_t t22 = (t2 + 1) % 3;

					//vertices in the first triangle that make up the second side
					Vertex<NavNode*, NavConnection*>* v11 = triangleData[i + t1];
					Vertex<NavNode*, NavConnection*>* v12 = triangleData[i + t12];

					//vertices in the second triangle that make up the second side
					Vertex<NavNode*, NavConnection*>* v21 = triangleData[j + t2];
					Vertex<NavNode*, NavConnection*>* v22 = triangleData[j + t22];

					//are the two edges the same vertices (check for wrong order additionally)
					if ((v11 == v21 && v12 == v22) || (v11 == v22 && v12 == v21))
					{
						bool duplicate = false;

						//iterate through all edges in the first vertex, searching for the edge that ends at the second vertex
						for (size_t e1 = 0; e1 < v11->edges.size(); e1++)
						{
							//store the edge in a temporary variable for performance and readability
							Edge<NavNode*, NavConnection*>* edge = &v11->edges[e1];

							//if the edge is the correct edge
							if (edge->end == v12)
							{
								//check that an edge with the same vertices hasn't already been found
								if (duplicate)
								{
									delete edge->value;
									v11->edges.erase(v11->edges.begin() + e1);
								}
								else
								{
									edge->value->sharing = true;
									duplicate = true;
								}
							}
						}

						duplicate = false;

						//iterate through all edges in the second vertex, searching for the edge that ends at the first vertex
						for (size_t e2 = 0; e2 < v12->edges.size(); e2++)
						{
							//store the edge in a temporary variable for performance and readability
							Edge<NavNode*, NavConnection*>* edge = &v12->edges[e2];

							//if the edge is the correct edge
							if (edge->end == v11)
							{
								//check that an edge with the same vertices hasn't already been found
								if (duplicate)
								{
									delete edge->value;
									v12->edges.erase(v12->edges.begin() + e2);
								}
								else
								{
									edge->value->sharing = true;
									duplicate = true;
								}
							}
						}
					}
				}
			}
		}
	}
}



//loads navmesh data from a file
void NavMesh::load(char fileName[FILENAME_MAX])
{
	std::ifstream inFile;

	//attempt to open the file
	inFile.open(fileName);

	//if the file successfully opened
	if (inFile.is_open())
	{
		//containers of loaded mesh data
		std::vector<Vector2> vertices;
		std::vector<int> triangles;

		char command;

		//while the end of the file hasn't been reached
		while (!inFile.eof())
		{
			//recieve the next command from the file
			inFile >> command;

			//allows the stream to ignore spaces
			char g = ' ';

			if (command == 'v') //vertex command, 2 floats incoming
			{
				//store the x and y coordinates
				float x, y;

				inFile >> x;
				inFile >> y;

				//add the loaded vector to the vertices list
				vertices.push_back(Vector2(x, y));
			}
			else if (command == 't') //triangle command, three indices of the vertices incoming
			{
				int v1, v2, v3;

				inFile >> v1;
				inFile >> v2;
				inFile >> v3;

				//add the three indices to the triangles list
				triangles.push_back(v1);
				triangles.push_back(v2);
				triangles.push_back(v3);

			}
		}

		//create a mesh from the loaded vertices and triangles
		createMesh(vertices, triangles);

		//calculate edges that are shared accross the mesh
		calculateSharedEdges();
	}

	//close the file so that other programs can access it safely
	inFile.close();
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
std::vector<Vector2> NavMesh::findRawPath(Vertex<NavNode*, NavConnection*>* start, Vertex<NavNode*, NavConnection*>* end, float E)
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



//prunes all unneccessary nodes with LOS checks
std::vector<Vector2> NavMesh::smoothPath(std::vector<Vector2> rawPath)
{
	//don't attempt to smooth a path without points
	if (rawPath.size() == 0)
	{
		return rawPath;
	}

	//iterate through all points in the path
	for (int i = 0; i < (int)rawPath.size() - 2; i++)
	{

		//attempt to try and skip the 2nd point in the path
		Vector2 to = rawPath[(size_t)i];
		Vector2 from = rawPath[(size_t)i+2];

		//line-of-sight flags
		bool exteriorIntersection = false;
		bool interiorIntersection = false;

		//iterate through all vertices in the NavMesh, searching for unshared edges
		for (size_t v = 0; v < data.vertices.size(); v++)
		{
			//store in a temporary value for readability and performance
			Vertex<NavNode*, NavConnection*>* vertex = data.vertices[v];

			//iterate through all edges in the vertex, searching for an unshared edge
			for (size_t e = 0; e < vertex->edges.size(); e++)
			{
				Edge<NavNode*, NavConnection*> edge = vertex->edges[e];

				//test if the boundary is intersecting the path between the nodes
				if (COLL_ENGINE->lineCollisionCheck(from, to, edge.start->data->position, edge.end->data->position))
				{
					//check if the line is an exterior/interior edge of the mesh
					if (edge.value->sharing)
					{
						interiorIntersection = true;
					}
					else
					{
						exteriorIntersection = true;
						break;
					}

				}
				
			}

			//break out of the loop if an intersecting exterior line was found
			if (exteriorIntersection)
			{
				break;
			}
		}

		//if two nodes can be connected without crossing the NavMesh's boundaries, remove the nodes between
		if (!exteriorIntersection && interiorIntersection)
		{
			//remove the node in front of the current node being examined
			rawPath.erase(rawPath.begin() + i + 1);

			//keep the iterator at the same number
			i--;
		}

	}
	
	return rawPath;
}



//draws the graph structure
void NavMesh::drawMesh(float nodeRadius, float connectionThickness, float r, float g, float b, float depth)
{
	//set the desired colour to draw the graph with
	appPtr->m_renderer2D->setRenderColour(r, g, b);

	//iterate through all triangles, drawing each one
	for (size_t i = 0; i < triangleData.size(); i+=3)
	{
		//store the three verts in temp variables
		Vertex<NavNode*, NavConnection*>* vert1 = triangleData[i];
		Vertex<NavNode*, NavConnection*>* vert2 = triangleData[i+1];
		Vertex<NavNode*, NavConnection*>* vert3 = triangleData[i+2];

		//store the three vectors in temp variables
		Vector2 v1 = vert1->data->position;
		Vector2 v2 = vert2->data->position;
		Vector2 v3 = vert3->data->position;

		//average vector of the triangle
		Vector2 A = (v1 + v2 + v3) / 3.0f;

		//relative vector for all vertices towards the average
		Vector2 A1 = v1 - A;
		Vector2 A2 = v2 - A;
		Vector2 A3 = v3 - A;

		float ratio = 0.9f;

		//travelling partially back from the average vector to the original vertex position
		Vector2 va1 = A + A1 * ratio;
		Vector2 va2 = A + A2 * ratio;
		Vector2 va3 = A + A3 * ratio;

		//draw the triangle
		appPtr->m_renderer2D->drawTriangle(va1.x, va1.y, va2.x, va2.y, va3.x, va3.y, depth);
	}

	//iterate through all vertices, drawing a circle for each one
	for (size_t i = 0; i < data.vertices.size(); i++)
	{
		Vertex<NavNode*, NavConnection*>* vertex = data.vertices[i];

		//iterate through all edges in the verty6ex, drawing a line for each one
		for (size_t j = 0; j < vertex->edges.size(); j++)
		{
			Edge<NavNode*, NavConnection*> edge = vertex->edges[j];

			//draw the edge
			appPtr->m_renderer2D->drawLine(edge.start->data->position.x, edge.start->data->position.y, edge.end->data->position.x, edge.end->data->position.y, connectionThickness, depth);
			
		}

		//draw the node
		appPtr->m_renderer2D->drawCircle(vertex->data->position.x, vertex->data->position.y, nodeRadius, depth);
	}

	//reset the colour
	appPtr->m_renderer2D->setRenderColour(1.0f, 1.0f, 1.0f);
}
