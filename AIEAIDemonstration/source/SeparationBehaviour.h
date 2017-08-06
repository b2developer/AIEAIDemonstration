#pragma once
#include "SteeringBehaviour.h"


/*
* class SeparationBehaviour
*
* a steering behaviour in the flocking series
* prevents boids from clumping together
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class SeparationBehaviour : public SteeringBehaviour
{
public:

	//radius at which boids are repelled from each other
	float neighbourhoodRadius = 50.0f;

	/*
	* SeparationBehaviour()
	* default constructor
	*/
	SeparationBehaviour() {}


	/*
	* ~SeparationBehaviour()
	* default destructor
	*/
	~SeparationBehaviour() {}


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