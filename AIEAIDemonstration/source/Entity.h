#pragma once
#include "Component.h"
#include "MathsLibrary.h"


//forward declarations
class Transform;
class Application2D;

/*
* class Script
* child class of Component
*
* a component that is another base object for 
* custom/uncommon implementations of behaviour
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Script : public Component
{
public:

	//pointer to the game
	Application2D* appPtr = nullptr;

	//pointer to the required transform component
	Transform* transform = nullptr;

	/*
	* Script()
	* default constructor
	*/
	Script() {}


	/*
	* ~Script()
	* default destructor
	*/
	~Script() {}


	/*
	* update
	* abstract function
	* overrides Component's update()
	*
	* gets called once per frame, updates
	* the game object's position using velocity
	*
	* @returns void
	*/
	virtual void update() override = 0;

};