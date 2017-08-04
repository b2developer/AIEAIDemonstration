#pragma once
#include "SteeringBehaviour.h"


/*
* class EvadeBehaviour
*
* a steering behaviour that steers away from a moving target
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class EvadeBehaviour : public SteeringBehaviour
{
public:

	//pointer to the moving target to steer away from
	SteeringBehaviourManager* targetBoid = nullptr;

	/*
	* EvadeBehaviour()
	* default constructor
	*/
	EvadeBehaviour() {}


	/*
	* ~EvadeBehaviour()
	* default destructor
	*/
	~EvadeBehaviour() {}


	/*
	* update
	* overrides SteeringBehaviour's update()
	*
	* called once per frame, returns
	* an acceleration vector to add
	*
	* @returns Vector2 - the vector to add to the overall velocity
	*/
	Vector2 update() override;

};