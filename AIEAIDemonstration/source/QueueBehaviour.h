#pragma once
#include "SteeringBehaviour.h"


/*
* class QueueBehaviour
*
* a steering behaviour that can replace cohesion
* rather than steering toward the average point of the boids
* this behaviour steers toward the average point of points
* placed behind the neighbours
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class QueueBehaviour : public SteeringBehaviour
{
public:

	//radius at which boids are moved closer to each other
	float neighbourhoodRadius = 50.0f;

	//the distance that each boid aims to be from other boids next to it in the queue
	float queueDistance = 50.0f;

	//frontal offset of the neighbourhood radius
	float neighbourhoodForwardDistance = 50.0f;

	/*
	* QueueBehaviour()
	* default constructor
	*/
	QueueBehaviour() {}


	/*
	* ~QueueBehaviour()
	* default destructor
	*/
	~QueueBehaviour() {}


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