#pragma once
#include "MathsLibrary.h"

/*
* this header contains several heurstic functions
* for the pathfinder to use when calculating a path
*
* there are multiple here because different functions
* work better on different structures of graph
*/

//define the square root of 2 constant
#define SQRT2 1.4142135f

/*
* ManhattanDistance
*
* a heurstic function used by the pathfinding map
* x diff + y diff
* works well on 4-way grids
*
* @returns float - the 'H' value
*/
float ManhattanDistance(Vector2 current, Vector2 end);


/*
* DiagonalDistance
*
* a heurstic function used by the pathfinding map
* sqrt(2) * min(dx, dy) + (max(dx, dy) - min(dx, dy))
* works well on 8-way grids
*
* @returns float - the 'H' value
*/
float DiagonalDistance(Vector2 current, Vector2 end);


/*
* EuclideanDistance
*
* a heurstic function used by the pathfinding map
* sqrt(d.x * d.x + d.y * d.y)
* works well on grids with any angle of pathfinding
*
* @returns float - the 'H' value
*/
float EuclideanDistance(Vector2 current, Vector2 end);
