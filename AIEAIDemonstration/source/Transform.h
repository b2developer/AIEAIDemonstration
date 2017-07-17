#pragma once
#include "Component.h"
#include "MathsLibrary.h"


/*
* class Transform
* child class of Component
*
* a component that stores and applies a 2D transformation
* that contains a translation, rotation and scaling
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Transform : public Component
{
public:

	//raw transformation data
	Vector2 translation = Vector2(0.0f, 0.0f);
	float rotation = 0.0f;
	Vector2 scale = Vector2(1.0f, 1.0f);

	/*
	* Transform()
	* default constructor
	*/
	Transform() {}


	/*
	* ~Transform()
	* default destructor
	*/
	~Transform() {}


	/*
	* update
	* overrides Component's update()
	*
	* necessary function to fill the interface
	* of the base object, does nothing
	*
	* @returns void
	*/
	void update() override {};
};