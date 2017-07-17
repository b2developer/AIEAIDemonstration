#pragma once
#include "Shape.h"
#include "Box.h"

/*
* class Circle
* child class of Shape
*
* a component that stores a radius for a circle
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Circle : public Shape
{
public:

	/*
	* NOTE:
	* scaling a circle independently in different
	* directions will have unknown consequences
	* 
	* they can be scaled uniformly however
	*/

	//radius parameter
	float radius = 1.0f;

	/*
	* Circle()
	* default constructor
	*/
	Circle() {}


	/*
	* ~Circle()
	* default destructor
	*/
	~Circle() {}


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