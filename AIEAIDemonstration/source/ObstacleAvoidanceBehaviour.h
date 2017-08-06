#pragma once
#include "SteeringBehaviour.h"


//ratio between a radian and a degree
#define DEG2RAD 0.0174533f

/*
* class ObstacleAvoidanceBehaviour
*
* a steering behaviour which steers away from obstacles
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class ObstacleAvoidanceBehaviour : public SteeringBehaviour
{
public:

	//the distance of the line that is used to detect future collisions
	float aheadDistance = 10.0f;

	/*
	* ObstacleAvoidanceBehaviour()
	* default constructor
	*/
	ObstacleAvoidanceBehaviour() {}


	/*
	* ~ObstacleAvoidanceBehaviour()
	* default destructor
	*/
	~ObstacleAvoidanceBehaviour() {}


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