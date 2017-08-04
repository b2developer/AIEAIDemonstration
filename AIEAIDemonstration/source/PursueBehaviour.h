#pragma once
#include "SteeringBehaviour.h"


/*
* class PursueBehaviour
*
* a steering behaviour that steers towards a moving target
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class PursueBehaviour : public SteeringBehaviour
{
public:

	//pointer to the moving target to steer towards
	SteeringBehaviourManager* targetBoid = nullptr;

	/*
	* PursueBehaviour()
	* default constructor
	*/
	PursueBehaviour() {}


	/*
	* ~PursueBehaviour()
	* default destructor
	*/
	~PursueBehaviour() {}


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