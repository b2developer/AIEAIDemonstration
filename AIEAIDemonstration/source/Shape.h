#pragma once
#include "Component.h"
#include "MathsLibrary.h"
#include <vector>


//acceptable limits
#define MIN -1000000000.0f;
#define MAX 1000000000.0f;

//forward declaration
class Transform;


/*
* struct Range
*
* represents all numbers between a minimum and a maximum
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
struct Range
{
	float min = 0.0f;
	float max = 0.0f;

	/*
	* Range()
	* default constructor
	*/
	Range() {}


	/*
	* Range(float _min, float _max)
	* constructor, immediately assigns values
	*
	* @param float _min - the minimum value
	* @param float _max - the maximum value
	*/
	Range(float _min, float _max) : min(_min), max(_max) {}
};


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


	/*
	* project
	* abstract function
	*
	* returns the lowest and highest dot products
	* from all points on the shape with the normal
	* 
	* @param Vector2 normal - the normal to project on
	* @returns Range - the lowest and highest dot products
	*/
	virtual Range project(Vector2 normal) = 0;


	/*
	* getNormals
	* abstract function
	*
	* gets normals from this to use in the projection phase
	* of the seperation axis theorem
	*
	* @param Shape* other - the other shape being tested for collisions
	* @returns std::vector<Vector2> - a list of the normals to be used in the projection
	*/
	virtual std::vector<Vector2> getNormals(Shape* other) = 0;
};