#pragma once
#include "MathsLibrary.h"
#include "Graph.h"
#include <vector>


//forward declarations for the triangle class to utilise
class NavMeshVertex;
class NavMeshEdge;
class NavMeshTriangleEdge;

/*
* class NavMeshTriangle
*
* a triangle that belongs to a navmesh, holds
* three NavMeshVertices and the edges connecting
* them
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class NavMeshTriangle
{
public:

	//coordinates of the triangle
	Vector2 position = Vector2(0,0);

	//flag indicating if A* has already addressed this node in the current search
	bool inOpen = false;

	//flag indicating if A* has already branched out using this node as a root in the current search
	bool visited = false;

	//current cost to move to this triangle from the starting triangle, determined by A*
	float gScore = INFINITY;

	//radius of the biggest circle that will fit inside the triangle
	float inCircleRadius = 0.0f;

	//the triangle that A* came from to get to this triangle
	NavMeshTriangle* previous = nullptr;

	//the three vertices involved in the triangle
	std::vector<NavMeshVertex*> vertices;

	//the three edges involved in the triangle
	std::vector<NavMeshTriangleEdge*> edges;

	/*
	* NavMeshTriangle()
	* default constructor
	*/
	NavMeshTriangle() {}


	/*
	* ~NavMeshTriangle()
	* default destructor
	*/
	~NavMeshTriangle() {}

};



/*
* class NavMeshVertex
*
* a vertex that belongs to a navmesh
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class NavMeshVertex
{
public:

	//coordinates of the vertex
	Vector2 position;

	/*
	* NavMeshVertex()
	* default constructor
	*/
	NavMeshVertex() {}


	/*
	* ~NavMeshVertex()
	* default destructor
	*/
	~NavMeshVertex() {}

};



/*
* class NavMeshTriangleEdge
*
* stores the connection between vertices
* on a triangle, the vertices may belong to multiple
* triangles in which case the edge is marked as shared
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class NavMeshTriangleEdge
{
public:

	//pointer to the vertex that this edge belongs to
	Vertex<NavMeshTriangle*, NavMeshEdge*>* triangle = nullptr;

	//container for triangles
	NavMeshVertex* vert1 = nullptr;
	NavMeshVertex* vert2 = nullptr;

	bool shared = false;

	/*
	* NavMeshTriangleEdge()
	* default constructor
	*/
	NavMeshTriangleEdge() {}


	/*
	* ~NavMeshTriangleEdge()
	* default destructor
	*/
	~NavMeshTriangleEdge() {}

};



/*
* class NavMeshEdge
*
* stores the connection between two triangles
* holds a cost, which the A* algorithm uses
* to determine the shortest path
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class NavMeshEdge
{
public:

	//container for triangles
	NavMeshTriangle* start = nullptr;
	NavMeshTriangle* end = nullptr;

	//g-score added if this edge is used
	float cost = 0.0f;

	/*
	* NavMeshEdge()
	* default constructor
	*/
	NavMeshEdge() {}


	/*
	* ~NavMeshEdge()
	* default destructor
	*/
	~NavMeshEdge() {}

};

