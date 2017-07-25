#pragma once
#include "Graph.h"
#include "MathsLibrary.h"
#include "PathStructure.h"
#include <vector>


/*
* ManhattanDistance
*
* a heurstic function used by the pathfinding map
* x diff + y diff
* works well on 4-way grids
*
* @returns float - the 'H' value
*/
float ManhattanDistance(Vector2 current, Vector2 end)
{
	return abs(end.x - current.x) + abs(end.y - current.y);
}


/*
* ManhattanDistance
*
* a heurstic function used by the pathfinding map
* x diff + y diff
* works well on 4-way grids
*
* @returns float - the 'H' value

float ManhattanDistance(Vector2 current, Vector2 end)
{
	return abs(end.x - current.x) + abs(end.y - current.y);
}
*/


/*
* class PathfindingMap
*
* holds a pathfinding graph structure, dynamically manages it
* can calculate the shortest path between two nodes on the map
*
* uses a completely customisable heurstic function
*
* responsible for cleaning up the pathfinding structure after
* A* pathfinding calculations are run on it
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class PathfindingMap
{
public:

	//heurstic function pointer
	float(*heursticFunction)(Vector2 current, Vector2 end);

	//the graph structure to transverse with A*
	Graph<PathNode*, PathConnection*> data;

	/*
	* PathfindingMap()
	* default constructor 
	*/
	PathfindingMap() {}


	/*
	* ~PathfindingMap()
	* destructor, deletes all path nodes and path connections
	*/
	~PathfindingMap();


	/*
	* calculateAStarPath
	*
	* runs the A* pathfinding algorithm on the graph
	* to find the shortest path between the start and end nodes
	*
	* the path returned is raw data off the graph, optimisations
	* could be made to the path to reduce the amount of vectors in it
	* or make it smoother
	*
	* @param Vertex<PathNode*, PathConnection*>* start - the starting node
	* @param Vertex<PathNode*, PathConnection*>* end - the target node
	* @returns std::vector<Vector2> - a list of vectors that make up the path
	a
	*/
	std::vector<Vector2> calculateAStarPath(Vertex<PathNode*, PathConnection*>* start, Vertex<PathNode*, PathConnection*>* end);


};