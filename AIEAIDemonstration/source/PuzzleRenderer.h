#pragma once
#include "Renderer.h"
#include "Box.h"
#include "Puzzle.h"
#include <vector>


//forward declaration
class Application2D;
class Transform;

/*
* class PuzzleRenderer
* child class of Renderer
*
* a component that renders a primitive when required
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class PuzzleRenderer : public Renderer
{
public:

	//pointer to the transform
	Transform* transform = nullptr;

	//the actual puzzle object
	Puzzle* puzzle;

	//speed of pieces when interpolating between positions
	float movementSpeed = 1.0f;

	/*
	* PuzzleRenderer()
	* default constructor
	*/
	PuzzleRenderer() {}


	/*
	* ~PuzzleRenderer()
	* default destructor
	*/
	~PuzzleRenderer() {};


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