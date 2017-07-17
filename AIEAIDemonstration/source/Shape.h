#pragma once
#include "Component.h"
#include "MathsLibrary.h"


//forward declaration
class Transform;

/*
* class Shape
* child class of Component
*
* a component that is a base object for different types of shapes
* eg. Box, Circle and Polygon
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Shape : public Component
{
public:

	//transformation matrix associated with the shape
	Transform* transform = nullptr;

	/*
	* Shape()
	* default constructor
	*/
	Shape() {}


	/*
	* ~Shape()
	* virtual function
	* default destructor
	*/
	virtual ~Shape() {}


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