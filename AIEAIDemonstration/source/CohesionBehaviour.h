#pragma once
#include "SteeringBehaviour.h"


/*
* class CohesionBehaviour
*
* a steering behaviour in the flocking series
* forces boids to move closer together
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class CohesionBehaviour : public SteeringBehaviour
{
public:

	//radius at which boids are moved closer to each other
	float neighbourhoodRadius = 50.0f;

	/*
	* CohesionBehaviour()
	* default constructor
	*/
	CohesionBehaviour() {}


	/*
	* ~CohesionBehaviour()
	* default destructor
	*/
	~CohesionBehaviour() {}


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