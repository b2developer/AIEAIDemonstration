#pragma once
#include "Renderer.h"
#include "Shape.h"
#include <vector>


//forward declaration
class Application2D;
class Transform;

/*
* class PrimitiveRenderer
* child class of Renderer
*
* a component that renders a primitive when required
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class PrimitiveRenderer : public Renderer
{
public:

	//pointer to the transform
	Transform* transform = nullptr;

	//the primitive to draw
	Shape* shape = nullptr;

	//the colour of the primitive to render
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	
	//the depth of the rendered primitive
	float depth = 0.0f;

	/*
	* PrimitiveRenderer()
	* default constructor
	*/
	PrimitiveRenderer() {}


	/*
	* ~PrimitiveRenderer()
	* default destructor
	*/
	~PrimitiveRenderer() {};


	/*
	* update
	* overrides Renderer's update()
	*
	* gets called once per frame when the
	* gameobject is called upon to be drawn
	*
	* @returns void
	*/
	void update() override;
};