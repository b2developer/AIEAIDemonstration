#pragma once
#include "Shape.h"
#include <vector>


/*
* class Polygon
* child class of Shape
*
* a component that stores any number of coordinates
* that form a perimeter around an area - a polygon
*
* ordering of connections:
*
* {1,2,3,4,5,6}
*
* lines formed by points in this order
* 1 -> 2
* 2 -> 3
* 3 -> 4
* 4 -> 5
* 5 -> 6
* 6 -> 1
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Polygon : public Shape
{
public:

	//container for the points
	std::vector<Vector2> points;

	/*
	* Polygon()
	* default constructor
	*/
	Polygon() {}


	/*
	* ~Polygon()
	* default destructor
	*/
	~Polygon() {}


	/*
	* update
	* virtual function
	* overrides Component's update()
	*
	* necessary function to fill the interface
	* of the base object, does nothing
	*
	* @returns void
	*/
	virtual void update() override {};
};