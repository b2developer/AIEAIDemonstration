#pragma once
#include "SteeringBehaviour.h"


/*
* class WanderBehaviour
*
* a steering behaviour that simulates smoothed random movement
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class WanderBehaviour : public SteeringBehaviour
{
public:

	//random vector from the previous frame
	Vector2 randomVector = Vector2(0, 0);

	//the radius of the wander circle in front of the boid, larger radius give more change in movement
	float wanderRadius = 1.0f;

	//the distance of the circle in front of the boid, smooths the movement
	float wanderDistance = 1.0f;

	//the magnitude of the random vector that can be added to the 
	float jitterAmount = 1.0f;

	/*
	* WanderBehaviour()
	* default constructor
	*/
	WanderBehaviour() {}


	/*
	* ~WanderBehaviour()
	* default destructor
	*/
	~WanderBehaviour() {}


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