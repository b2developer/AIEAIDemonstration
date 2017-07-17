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
};