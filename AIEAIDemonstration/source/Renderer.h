#pragma once
#include "Component.h"
#include "Texture.h"
#include "Box.h"


//forward declaration
class Application2D;

/*
* class Renderer
* child class of Component
*
* a component that stores a texture
* and renders it when required
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Renderer : public Component
{
public:

	//pointer to the game
	Application2D* appPtr = nullptr;

	//the region to draw the texture to
	Box* box = nullptr;

	//the texture data to draw
	aie::Texture* texture = nullptr;

	/*
	* Renderer()
	* default constructor
	*/
	Renderer() {}


	/*
	* ~Renderer()
	* destructor, removes the texture from memory (if one has been allocated)
	*/
	virtual ~Renderer();


	/*
	* load
	*
	* attempts to load texture data from the file path
	* into memory for the renderer to use
	*
	* @param char fileName[FILENAME_MAX] - c-string of the file path
	* @returns void
	*/
	virtual void load(char fileName[FILENAME_MAX]);


	/*
	* update
	* virtual function
	* overrides Component's update()
	*
	* gets called once per frame when the
	* gameobject is called upon to be drawn
	*
	* @returns void
	*/
	virtual void update() override;
};