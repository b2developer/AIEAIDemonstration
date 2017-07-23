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
* for a system that can think
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
	* virtual function
	* default destructor
	*/
	virtual ~Agent() {}


	/*
	* update
	* abstract function
	* overrides Component's update()
	*
	* gets called once per frame, does nothing
	* exists to satisfy the abstract function it is overriding
	*
	* @returns void
	*/
	virtual void update() override = 0;

};