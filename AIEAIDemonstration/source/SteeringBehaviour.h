#pragma once
#include "MathsLibrary.h"


//forward declaration
class SteeringBehaviourManager;

/*
* class SteeringBehaviour
*
* a component that serves as an abstract interface
* for a system that can think
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class SteeringBehaviour
{
public:

	//pointer to the steering behaviour manager holding this behaviour
	SteeringBehaviourManager* sbm = nullptr;

	//multiplier for the acceleration vector returned
	float weight = 1.0f;

	/*
	* SteeringBehaviour()
	* default constructor
	*/
	SteeringBehaviour() {}


	/*
	* ~SteeringBehaviour()
	* virtual function
	* default destructor
	*/
	virtual ~SteeringBehaviour() {}


	/*
	* update
	* abstract function
	*
	* called once per frame, returns 
	* an acceleration vector to add
	*
	* @returns Vector2 - the vector to add to the overall velocity
	*/
	virtual Vector2 update() = 0;

};