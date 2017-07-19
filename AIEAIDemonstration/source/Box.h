#pragma once
#include "Shape.h"


/*
* class Box
* child class of Shape
*
* a component that stores two coordinates of the bottom-left
* and top-right corners of a box
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Box : public Shape
{
public:

	//corner coordinates
	Vector2 min = Vector2(0, 0);
	Vector2 max = Vector2(0, 0);

	/*
	* Box()
	* default constructor
	*/
	Box() {}


	/*
	* ~Box()
	* default destructor
	*/
	~Box() {}


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