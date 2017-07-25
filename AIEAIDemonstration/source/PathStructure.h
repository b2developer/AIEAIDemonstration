#pragma once
#include "MathsLibrary.h"

#pragma region PathNode
/*
* class PathNode
*
* describes a possible destination in a pathfinding graph,
* tracks the score it takes to travel to this path node from
* the starting node, track's it's connection to the starting node
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class PathNode
{

public:

	//position of the node
	Vector2 position = Vector2(0.0f, 0.0f);
	
	//cost to travel to this node from the starting node
	float gScore = INFINITY;

	//previous node along the optimal path from the start to here
	PathNode* previous = nullptr; 

	//flag indicating if A* has placed this node in the open list
	bool inOpen = false;

	//flag indicating if A* has visited this node already
	bool visited = false;



	/*
	* PathNode()
	* default constructor
	*/
	PathNode() {}


	/*
	* ~PathNode()
	* default destructor
	*/
	~PathNode() {}

};
#pragma endregion



#pragma region PathConnection
/*
* class PathConnection
*
* describes a possible connection between two path nodes in a pathfinding graph,
* tracks the score added if transversed down this connection
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class PathConnection
{

public:

	//cost to travel to travel accross this connection
	float cost = 0.0f;

	/*
	* PathConnection()
	* default constructor
	*/
	PathConnection() {}


	/*
	* ~PathConnection()
	* default destructor
	*/
	~PathConnection() {}

};
#pragma endregion
