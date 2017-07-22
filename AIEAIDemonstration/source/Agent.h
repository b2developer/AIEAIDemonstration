#pragma once
#include "Component.h"


//forward declarations
class Transform;
class Application2D;

/*
* class Agent
* child class of Component
*
* a component that serves as an abstract interface
* for a 
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Agent : public Component
{
public:

	//pointer to the application
	Application2D* appPtr = nullptr;

	//pointer to the required transform component
	Transform* transform = nullptr;

	/*
	* Agent()
	* default constructor
	*/
	Agent() {}


	/*
	* ~Agent()
	* default destructor
	*/
	~Agent() {}


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