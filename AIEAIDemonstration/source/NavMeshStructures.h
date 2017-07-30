#pragma once
#include "MathsLibrary.h"


//forward declarations for the triangle class to utilise
class NavMeshVertex;
class NavMeshEdge;

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

	//flag indicating if A* has already addressed this node in the current search
	bool open = false;

	//current cost to move to this triangle from the starting triangle, determined by A*
	float gScore = INFINITY;

	//the three vertices involved in the triangle
	std::vector<NavMeshVertex*> vertices;

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

