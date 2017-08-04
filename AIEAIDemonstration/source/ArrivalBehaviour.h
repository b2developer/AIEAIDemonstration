#pragma once
#include "SteeringBehaviour.h"


/*
* class ArrivalBehaviour
*
* a steering behaviour that aims to slow down movement
* when approaching the destination to prevent overshooting
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class ArrivalBehaviour : public SteeringBehaviour
{
public:

	//pointer to the vector to navigate away from
	Vector2* target = nullptr;

	//radius to slow down after entering
	float arrivalRadius = 1.0f;

	/*
	* ArrivalBehaviour()
	* default constructor
	*/
	ArrivalBehaviour() {}


	/*
	* ~ArrivalBehaviour()
	* default destructor
	*/
	~ArrivalBehaviour() {}


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