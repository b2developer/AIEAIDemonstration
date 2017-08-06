#pragma once
#include "SteeringBehaviour.h"


/*
* class AlignmentBehaviour
*
* a steering behaviour in the flocking series
* boids steer towards the average velocity of the neighbourhood
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class AlignmentBehaviour : public SteeringBehaviour
{
public:

	//radius at which boids observe other boids
	float neighbourhoodRadius = 50.0f;
	
	//frontal offset of the neighbourhood radius
	float neighbourhoodForwardDistance = 50.0f;

	/*
	* AlignmentBehaviour()
	* default constructor
	*/
	AlignmentBehaviour() {}


	/*
	* ~AlignmentBehaviour()
	* default destructor
	*/
	~AlignmentBehaviour() {}


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