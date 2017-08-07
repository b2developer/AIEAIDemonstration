#include "NavMesh.h"
#include "Application2D.h"

//collision math
#include "CollisionEngine.h"
#include "Polygon.h"

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



//calculates the maximum shoulder width for each triangle in the mesh
void NavMesh::calculateInCircles()
{
	//iterate through all triangles in the mesh, calculating the area, perimeter and inCircle radius of each
	for (size_t i = 0; i < data.vertices.size(); i++)
	{
		//store in a temporary value for performance and readability
		NavMeshTriangle* triangle = data.vertices[i]->data;

		//get all side lengthss
		float a = (triangle->edges[0]->vert2->position - triangle->edges[0]->vert1->position).magnitude();
		float b = (triangle->edges[1]->vert2->position - triangle->edges[1]->vert1->position).magnitude();
		float c = (triangle->edges[2]->vert2->position - triangle->edges[2]->vert1->position).magnitude();

		//calculate total length (perimieter)
		float p = a + b + c;

		float s = p / 2;

		//area using heron's formula
		float area = sqrt(s * (s - a) * (s - b) * (s - c));

		//get radius using perimeter and area
		triangle->inCircleRadius = (2 * area) / p;

		int e = 0;

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

		//calculate the maximum shoulder width of an agent crossing each triangle
		calculateInCircles();
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
std::vector<NavMeshTriangle*> NavMesh::findRawPath(Vertex<NavMeshTriangle*, NavMeshEdge*>* start, Vertex<NavMeshTriangle*, NavMeshEdge*>* end, float E, float shoulderWidth)
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

			//check that the triangle can contain the shoulder radius
			if (edge.end->data->inCircleRadius <= shoulderWidth)
			{
				continue;
			}

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



//prunes all unneccessary nodes with LOS checks
std::vector<Vector2> NavMesh::smoothPath(std::vector<NavMeshTriangle*> rawPath, Vector2 start, Vector2 end, float shoulderWidth)
{
	//return an empty list if the path is empty
	if (rawPath.size() == 0)
	{
		return std::vector<Vector2>();
	}
	
	//get the portals and their midpoints (portals are edges that are involved in the path)
	std::vector<NavMeshTriangleEdge*> portals;
	std::vector<Vector2> midpoints;

	/*
	* if a path is found, consecutive triangles in the path must have at exactly one common edge
	* otherwise the NavMesh has loaded a mesh that technically isn't a mesh
	* the code below assumes that a mesh with the correct topology was loaded
	*/

	midpoints.push_back(start);

	//iterate through all triangles in the path, searching for portals
	for (size_t i = 0; i < rawPath.size() - 1; i++)
	{
		//store consecutive triangles in temp variables
		NavMeshTriangle* tri1 = rawPath[i];
		NavMeshTriangle* tri2 = rawPath[i+1];

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

		portals.push_back(sharedEdge);

		//average point between edges
		Vector2 A = (sharedEdge->vert1->position + sharedEdge->vert2->position) / 2.0f;

		midpoints.push_back(A);

		//relative vectors from the vertices to the midpoint of the edge
		Vector2 EA1 = (A - sharedEdge->vert1->position).normalised();
		Vector2 EA2 = (A - sharedEdge->vert2->position).normalised();

		//move the edges in, this means that an agent with a shoulder radius will never be outside of the mesh after path smoothing
		sharedEdge->shrunkEdge1 = sharedEdge->vert1->position + EA1 * shoulderWidth;
		sharedEdge->shrunkEdge2 = sharedEdge->vert2->position + EA2 * shoulderWidth;
	}
	
	midpoints.push_back(end);

	std::vector<Vector2> path;

	//add the starting position to the path list
	path.push_back(start);
	
	bool finishReached = false; //flag indicating that the finish was iterated over
	bool finishReachable = false; //flag indicating a direct LOS to the finish

	//iterate through all the points in the path
	while (!finishReached || !finishReachable)
	{
		Vector2 lineStart = midpoints[0];

		finishReached = false;
		finishReachable = true;

		//early exit if there isn't enough data for LOS tests
		if (portals.size() < 1 || midpoints.size() <= 1)
		{
			finishReached = true;
			finishReachable = true;
			break;
		}


		//iterate through all the portals in front 
		for (size_t j = 2; j < midpoints.size(); j++)
		{
			Vector2 lineEnd = midpoints[j];

			bool failed = false;

			//early exit if there isn't enough data for LOS tests
			if (portals.size() < 1 || midpoints.size() <= 1)
			{
				finishReached = true;
				finishReachable = true;
				failed = true;
				break;
			}

			//has the finish been iterated through
			if (j >= midpoints.size() - 1)
			{
				finishReached = true;
				finishReachable = true;
			}

			//iterate through all portals in-between the midpoints
			for (size_t k = 0; k < j - 1 && k < portals.size(); k++)
			{
				//store in a temporary value for performance and readability
				NavMeshTriangleEdge* testPortal = portals[k];

				Vector2 portalStart = testPortal->shrunkEdge1;
				Vector2 portalEnd = testPortal->shrunkEdge2;

				//don't do anything if the line still intersects
				if (COLL_ENGINE->lineCollisionCheck(lineStart, lineEnd, portalStart, portalEnd, 0.0f))
				{
					continue;
				}

				finishReachable = false;

				int jm = 0;

				//edge case handling
				if (finishReached)
				{
					jm = -1;
				}

				//get the two portals that the bend contains
				NavMeshTriangleEdge* portal = portals[j - 1 + jm];

				Vector2 bendPosition;

				//get the last midpoint that was successful in the LOS tests
				path.push_back(midpoints[j - 1]);
				bendPosition = midpoints[j - 1];

				//remove all midpoints from the start of the test to the last successful test
				if (finishReached)
				{
					midpoints.erase(midpoints.begin(), midpoints.begin() + j);
				}
				else
				{
					midpoints.erase(midpoints.begin(), midpoints.begin() + j - 1);
				}

				//edge case handling for removing portals
				if (portals.size() > 1)
				{
					if (finishReached)
					{
						portals.erase(portals.begin(), portals.begin() + portals.size() - 2);	
					}
					else
					{
						portals.erase(portals.begin(), portals.begin() + j - 1);
					}
				}
				else
				{
					//erase() can't remove one node, clear() can
					portals.clear();
				}
	
				//get the next point to test
				midpoints[0] = bendPosition;

				failed = true;

				break;

			}

			if (failed)
			{
				break;
			}
		}
	}

	//add the destination to the path list
	path.push_back(end);

	return path;
}



//combines pieces of the pathfinding and path-smoothing algorithms above to generate a smooth path through the nav mesh
std::vector<Vector2> NavMesh::findPath(Vector2 start, Vector2 end, float shoulderWidth)
{
	
	//the polygon component is temporarily used here to check for containment of the points inside triangles

	bool found = false;

	size_t si = 0;
	size_t fi = 0;

	//iterate through all the triangles, testing for containment of the starting point
	for (size_t i = 0; i < data.vertices.size(); i++)
	{
		NavMeshTriangle* triangle = data.vertices[i]->data;

		Transform* null1 = new Transform();
		Transform* null2 = new Transform();

		Shape* trianglePoly = new Polygon();

		//add the three points of the triangle to the polygon (triangle poly)
		((Polygon*)trianglePoly)->points.push_back(triangle->vertices[0]->position);
		((Polygon*)trianglePoly)->points.push_back(triangle->vertices[1]->position);
		((Polygon*)trianglePoly)->points.push_back(triangle->vertices[2]->position);

		trianglePoly->transform = null1;

		Shape* pointPoly = new Polygon();

		//add the starting point to the polygon (point poly)
		((Polygon*)pointPoly)->points.push_back(start);

		pointPoly->transform = null2;

		//test for a collision
		if (COLL_ENGINE->simpleCollisionCheck(trianglePoly, pointPoly))
		{
			found = true;

			si = i;

			//remove the polygons after use
			delete trianglePoly;
			delete pointPoly;
			delete null1;
			delete null2;

			break;
		}

		//remove the polygons after use
		delete trianglePoly;
		delete pointPoly;
		delete null1;
		delete null2;
	}

	//the starting point isn't in the NavMesh, return an empty list
	if (!found)
	{
		return std::vector<Vector2>();
	}

	found = false;

	//iterate through all the triangles, testing for containment of the ending point
	for (size_t i = 0; i < data.vertices.size(); i++)
	{
		NavMeshTriangle* triangle = data.vertices[i]->data;

		Transform* null1 = new Transform();
		Transform* null2 = new Transform();

		Shape* trianglePoly = new Polygon();

		//add the three points of the triangle to the polygon (triangle poly)
		((Polygon*)trianglePoly)->points.push_back(triangle->vertices[0]->position);
		((Polygon*)trianglePoly)->points.push_back(triangle->vertices[1]->position);
		((Polygon*)trianglePoly)->points.push_back(triangle->vertices[2]->position);

		trianglePoly->transform = null1;

		Shape* pointPoly = new Polygon();

		//add the ending point to the polygon (point poly)
		((Polygon*)pointPoly)->points.push_back(end);

		pointPoly->transform = null2;

		//test for a collision
		if (COLL_ENGINE->simpleCollisionCheck(trianglePoly, pointPoly))
		{
			found = true;

			fi = i;

			//remove the polygons after use
			delete trianglePoly;
			delete pointPoly;
			delete null1;
			delete null2;

			break;
		}

		//remove the polygons after use
		delete trianglePoly;
		delete pointPoly;
		delete null1;
		delete null2;
	}
	
	//the end point isn't in the NavMesh, return an empty list
	if (!found)
	{
		return std::vector<Vector2>();
	}

	std::vector<NavMeshTriangle*> list = findRawPath(data.vertices[si], data.vertices[fi], 1.0f, shoulderWidth);
	std::vector<Vector2> path = smoothPath(list, start, end, shoulderWidth);

	return path;
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

		float ratio = 1.0f;

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
