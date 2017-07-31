#include "NavMesh.h"
#include "Application2D.h"

//collision math
#include "CollisionEngine.h"

//headers for file I/O
#include <iostream>
#include <fstream>

//destructor, deletes all vertices, edges and triangle data
NavMesh::~NavMesh()
{
	//iterate through all triangle vertices, deleting each one
	for (size_t i = 0; i < triangleVerts.size(); i++)
	{
		delete triangleVerts[i];
	}

	//iterate through all triangle edges, deleting each one
	for (size_t i = 0; i < triangleEdges.size(); i++)
	{
		delete triangleEdges[i];
	}

	//iterate through all vertices in the graph, deleting each one
	for (size_t i = 0; i < data.vertices.size(); i++)
	{
		//store the data in a temporary value for performance and readability
		Vertex<NavMeshTriangle*, NavMeshEdge*>* vertex = data.vertices[i];

		//iterate through all edges in the vertex, deleting the data at each one
		for (size_t j = 0; j < vertex->edges.size(); j++)
		{
			delete vertex->edges[j].value;
		}

		delete vertex->data;
	}

}


//creates graph data that matches the vertex and triangle list
void NavMesh::createMesh(std::vector<Vector2> vertices, std::vector<int> triangles)
{
	//container for the vertex
	std::vector<NavMeshVertex*> navVertices;

	//iterate through the list of Vector2's (vertices), adding a vertex to the graph for each one
	for (size_t i = 0; i < vertices.size(); i++)
	{
		//create a nav vertex with the correct position
		NavMeshVertex* navNode = new NavMeshVertex();
		navNode->position = vertices[i];

		navVertices.push_back(navNode);
	}

	triangleVerts = navVertices;

	//iterate through the list of triangles, adding three connections for each
	for (size_t i = 0; i < triangles.size(); i += 3)
	{
		//get the three nav vertices involved in the triangle
		NavMeshVertex* vert1 = navVertices[triangles[i] - 1];
		NavMeshVertex* vert2 = navVertices[triangles[i+1] - 1];
		NavMeshVertex* vert3 = navVertices[triangles[i+2] - 1];

		//three edges are formed, one per vertex
		NavMeshTriangleEdge* n12 = new NavMeshTriangleEdge();
		NavMeshTriangleEdge* n23 = new NavMeshTriangleEdge();
		NavMeshTriangleEdge* n31 = new NavMeshTriangleEdge();

		//give all triangle edges their vertices
		n12->vert1 = vert1;
		n12->vert2 = vert2;

		n23->vert1 = vert2;
		n23->vert2 = vert3;

		n31->vert1 = vert3;
		n31->vert2 = vert1;

		//add at the edges to the edges container
		triangleEdges.push_back(n12);
		triangleEdges.push_back(n23);
		triangleEdges.push_back(n31);

		/*
		* in order to draw the triangle mesh properly using the bootstrap,
		* the points must be in counter-clockwise order otherwise bootstrap
		* will render the triangle facing away from the viewport
		* the code below ensures that the list of vertices are always in the
		* correct order to be drawn to the viewport correctly
		*/

		//sort the vertices based on their y axis
		NavMeshVertex* topVertex = nullptr;
		NavMeshVertex* centreVertex = nullptr;
		NavMeshVertex* bottomVertex = nullptr;

		//test case for the first vertex
		if (vert1->position.y < fmaxf(vert2->position.y, vert3->position.y) && vert1->position.y > fminf(vert2->position.y, vert3->position.y))
		{
			centreVertex = vert1;

			//test if vert2 is the bottom
			if (vert2->position.y < centreVertex->position.y)
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
		else if (vert2->position.y < fmaxf(vert1->position.y, vert3->position.y) && vert2->position.y > fminf(vert1->position.y, vert3->position.y))
		{
			centreVertex = vert2;

			//test if vert1 is the bottom
			if (vert1->position.y < centreVertex->position.y)
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
			if (vert1->position.y < centreVertex->position.y)
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

		NavMeshTriangle* triangle = new NavMeshTriangle();

		//store the three vectors in temp variables
		Vector2 v1 = vert1->position;
		Vector2 v2 = vert2->position;
		Vector2 v3 = vert3->position;

		//average vector of the triangle
		Vector2 A = (v1 + v2 + v3) / 3.0f;

		triangle->position = A;

		triangle->vertices.push_back(centreVertex);

		//which side of the line between the bottom and top vertices is the middle vertex on
		if (COLL_ENGINE->calculateSide(bottomVertex->position, topVertex->position, centreVertex->position) >= 0)
		{ 
			//left/middle
			triangle->vertices.push_back(bottomVertex);
			triangle->vertices.push_back(topVertex);
			
		}
		else
		{
			//right
			triangle->vertices.push_back(topVertex);
			triangle->vertices.push_back(bottomVertex);
			
		}

		//add the relevant edges to the triangle
		triangle->edges.push_back(n12);
		triangle->edges.push_back(n23);
		triangle->edges.push_back(n31);

		//add the triangle to the graph
		Vertex<NavMeshTriangle*, NavMeshEdge*>* vertex = data.addVertex(triangle);

		//link the triangle edges to the vertex in the graph
		n12->triangle = vertex;
		n23->triangle = vertex;
		n31->triangle = vertex;
	}
}



//calculates common edges between triangles in the mesh
void NavMesh::calculateSharedEdges()
{

	//iterate through all triangles, comparing them to every other triangle
	for (size_t i = 0; i < triangleEdges.size(); i++)
	{
		for (size_t j = 0; j < triangleEdges.size(); j ++)
		{
			//don't compare a triangle to itself
			if (i == j)
			{
				continue;
			}

			//store the edges in a temporary value for perforamcne and readability
			NavMeshTriangleEdge* navEdge1 = triangleEdges[i];
			NavMeshTriangleEdge* navEdge2 = triangleEdges[j];

			//compare the edges to see if they are the same, or are the reverse of each other
			if ((navEdge1->vert1 == navEdge2->vert1 && navEdge1->vert2 == navEdge2->vert2) || (navEdge1->vert1 == navEdge2->vert2 && navEdge1->vert2 == navEdge2->vert1))
			{
				navEdge1->shared = true;
				navEdge2->shared = true;

				//connect the corresponding triangles
				data.createConnection(navEdge1->triangle, navEdge2->triangle);

				//edges connecting two triangles together
				NavMeshEdge* navMeshEdge1 = new NavMeshEdge();

				//calculate the distance between the centres of each triangle
				float cost = (navEdge2->triangle->data->position - navEdge1->triangle->data->position).magnitude();

				navMeshEdge1->cost = cost;

				//give the edges that were just added their values
				navEdge1->triangle->edges[navEdge1->triangle->edges.size() - 1].value = navMeshEdge1;
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
void NavMesh::resetSearchedNodes(std::vector<Vertex<NavMeshTriangle*, NavMeshEdge*>*> open, std::vector<Vertex<NavMeshTriangle*, NavMeshEdge*>*> closed)
{
	//iterate through all open nodes, resetting them all
	for (size_t i = 0; i < open.size(); i++)
	{
		//temporary value for readability and performance
		NavMeshTriangle* tr = open[i]->data;

		//reset the values
		tr->gScore = INFINITY;
		tr->inOpen = false;
		tr->visited = false;
		tr->previous = nullptr;
	}

	//iterate through all closed nodes, resetting them all
	for (size_t i = 0; i < closed.size(); i++)
	{
		//temporary value for readability and performance
		NavMeshTriangle* tr = closed[i]->data;

		//reset the values
		tr->gScore = INFINITY;
		tr->inOpen = false;
		tr->visited = false;
		tr->previous = nullptr;
	}
}

//calculates the shortest path from the starting node to the ending node using the A* pathfinding algorithm
std::vector<NavMeshTriangle*> NavMesh::findRawPath(Vertex<NavMeshTriangle*, NavMeshEdge*>* start, Vertex<NavMeshTriangle*, NavMeshEdge*>* end, float E)
{
	//the open list
	std::vector<Vertex<NavMeshTriangle*, NavMeshEdge*>*> open;

	//the closed list, used to reset the nodes
	std::vector<Vertex<NavMeshTriangle*, NavMeshEdge*>*> closed;

	start->data->visited = true;
	start->data->gScore = 0.0f;

	open.push_back(start);

	//while the open list still contains 
	while (open.size() > 0)
	{

		//remember the node with the lowest 'F' score in the open list
		Vertex<NavMeshTriangle*, NavMeshEdge*>* best = open[0];
		size_t bestI = 0;

		//iterate through all nodes, finding the node with the smallest 'F' score
		for (size_t i = 1; i < open.size(); i++)
		{
			//store in a temp variable, saves performance and increases readability
			Vertex<NavMeshTriangle*, NavMeshEdge*>* node = open[i];

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
			//list of vertices that make up the path
			std::vector<NavMeshTriangle*> nodeList;

			NavMeshTriangle* current = best->data;

			//backtrack using the previous nodes until the start is reached
			while (current != start->data)
			{
				//add the node's position to the front of the list
				nodeList.insert(nodeList.begin(), current);

				//go to the previous node
				current = current->previous;
			}

			//add the starting node's position
			nodeList.insert(nodeList.begin(), current);

			//reset the lists of open and closed nodes
			resetSearchedNodes(open, closed);

			return nodeList;
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
			Edge<NavMeshTriangle*, NavMeshEdge*> edge = best->edges[i];

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

	return std::vector<NavMeshTriangle*>();

}



//prunes all unneccessary nodes with the funneling algorithm
std::vector<Vector2> NavMesh::funnelPath(std::vector<NavMeshTriangle*> rawPath, Vector2 start, Vector2 end)
{
	//get the portals (edges that are involved in the path)
	std::vector<NavMeshTriangleEdge*> portals;

	//list of boolean values indicating if the corresponding portal is alligned
	std::vector<bool> allignments;

	/*
	* if a path is found, consecutive triangles in the path must have at exactly one common edge
	* otherwise the NavMesh has loaded a mesh that technically isn't a mesh
	* the code below assumes that a mesh with the correct topology was loaded
	*/

	//iterate through all triangles in the path, searching for portals
	for (size_t i = 1; i < rawPath.size(); i++)
	{
		//store consecutive triangles in temp variables
		NavMeshTriangle* tri1 = rawPath[i - 1];
		NavMeshTriangle* tri2 = rawPath[i];

		//store the shared edge in a variable after it is found
		NavMeshTriangleEdge* sharedEdge = nullptr;

		//iterate through all edges in both triangles, searching for a common edge
		for (size_t e1 = 0; e1 < 3; e1++)
		{
			for (size_t e2 = 0; e2 < 3; e2++)
			{
				//temporary variables for performance and readability
				NavMeshTriangleEdge* navEdge1 = tri1->edges[e1];
				NavMeshTriangleEdge* navEdge2 = tri2->edges[e2];

				//are the edges identical or the reverse of each other
				if ((navEdge1->vert1 == navEdge2->vert1 && navEdge1->vert2 == navEdge2->vert2) || (navEdge1->vert1 == navEdge2->vert2 && navEdge1->vert2 == navEdge2->vert1))
				{
					sharedEdge = navEdge1;
					break;
				}
			}

			//break out of both loop if the edge is found
			if (sharedEdge != nullptr)
			{
				break;
			}
		}

		//directional vector from this previous node to this node
		Vector2 relative = (tri2->position - tri1->position).normalised();

		//directional vector from the previous node to the first vertice in the shared edge
		Vector2 first = (sharedEdge->vert1->position - tri1->position).normalised();

		//2D cross product
		float cross = relative.x * first.y - first.x * relative.y;

		portals.push_back(sharedEdge);
		allignments.push_back(cross > 0);
	}

	

	return std::vector<Vector2>();
}



std::vector<Vector2> NavMesh::findPath(Vector2 start, Vector2 end)
{
	return std::vector<Vector2>();
}

//draws the graph structure
void NavMesh::drawMesh(float nodeRadius, float connectionThickness, float r, float g, float b, float depth)
{

	//iterate through all triangles, drawing each one
	for (size_t i = 0; i < data.vertices.size(); i++)
	{
	
		//store in temporary variable for performance and readability
		Vertex<NavMeshTriangle*, NavMeshEdge*>* triangle = data.vertices[i];

		//store the three verts in temp variables
		NavMeshVertex* vert1 = triangle->data->vertices[0];
		NavMeshVertex* vert2 = triangle->data->vertices[1];
		NavMeshVertex* vert3 = triangle->data->vertices[2];

		//store the three vectors in temp variables
		Vector2 v1 = vert1->position;
		Vector2 v2 = vert2->position;
		Vector2 v3 = vert3->position;

		//average vector of the triangle
		Vector2 A = triangle->data->position;

		//relative vector for all vertices towards the average
		Vector2 A1 = v1 - A;
		Vector2 A2 = v2 - A;
		Vector2 A3 = v3 - A;

		float ratio = 0.95f;

		//travelling partially back from the average vector to the original vertex position
		Vector2 va1 = A + A1 * ratio;
		Vector2 va2 = A + A2 * ratio;
		Vector2 va3 = A + A3 * ratio;

		appPtr->m_renderer2D->setRenderColour(r, g, b);

		//draw the triangle
		appPtr->m_renderer2D->drawTriangle(va1.x, va1.y, va2.x, va2.y, va3.x, va3.y, depth);

		appPtr->m_renderer2D->setRenderColour(r * 0.5f, g * 0.5f, b * 0.5f);

		//iterate through all edges in the triangle, drawing each one
		for (size_t j = 0; j < triangle->edges.size(); j++)
		{
			Edge<NavMeshTriangle*, NavMeshEdge*> edge = triangle->edges[j];

			//draw a circle at both ends of the triangle connections
			appPtr->m_renderer2D->drawCircle(edge.start->data->position.x, edge.start->data->position.y, nodeRadius, depth - 0.5f);
			appPtr->m_renderer2D->drawCircle(edge.end->data->position.x, edge.end->data->position.y, nodeRadius, depth - 0.5f);

			appPtr->m_renderer2D->drawLine(edge.start->data->position.x, edge.start->data->position.y, edge.end->data->position.x, edge.end->data->position.y, connectionThickness, depth - 0.25f);
		}
	}

	//reset the colour
	appPtr->m_renderer2D->setRenderColour(1.0f, 1.0f, 1.0f);
}
