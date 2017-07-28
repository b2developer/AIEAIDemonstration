#pragma once
#include "MathsLibrary.h"

#pragma region NavNode
/*
* class NavNode
*
* describes a possible destination in a NavMesh,
* tracks the score it takes to travel to this path node from
* the starting node, track's it's connection to the starting node
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class NavNode
{

public:

	//position of the node
	Vector2 position = Vector2(0.0f, 0.0f);
	
	//cost to travel to this node from the starting node
	float gScore = INFINITY;

	//index of the node in the mesh
	size_t vertexIndex = 0;

	//previous node along the optimal path from the start to here
	NavNode* previous = nullptr;

	//flag indicating if A* has placed this node in the open list
	bool inOpen = false;

	//flag indicating if A* has visited this node already
	bool visited = false;

	/*
	* NavNode()
	* default constructor
	*/
	NavNode() {}


	/*
	* ~NavNode()
	* default destructor
	*/
	~NavNode() {}

};
#pragma endregion



#pragma region NavConnection
/*
* class NavConnection
*
* describes a possible connection between two path nodes in a NavMesh,
* tracks the score added if transversed down this connection
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class NavConnection
{

public:

	//cost to travel to travel accross this connection
	float cost = 0.0f;

	//flag indicating that the connection is owned by multiple triangles
	bool sharing = false;

	/*
	* NavConnection()
	* default constructor
	*/
	NavConnection() {}


	/*
	* ~NavConnection()
	* default destructor
	*/
	~NavConnection() {}

};
#pragma endregion
