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


	/*
	* project
	* overrides Shape's project(Shape* other, Vector2 normal)
	*
	* returns the lowest and highest dot products
	* from all points on the shape with the normal
	*
	* @param Vector2 normal - the normal to project on
	* @returns Range - the lowest and highest dot products
	*/
	Range project(Vector2 normal) override;


	/*
	* getNormals
	* overrides Shape's getNormals(Shape* other)
	*
	* gets normals from this to use in the projection phase
	* of the seperation axis theorem
	*
	* @param Shape* other - the other shape being tested for collisions
	* @returns std::vector<Vector2> - a list of the normals to be used in the projection
	*/
	std::vector<Vector2> getNormals(Shape* other);
};