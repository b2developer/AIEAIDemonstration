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

	//angle at which the feelers detect future collisions
	float feelerAngle = 45.0f;

	//scaling feeler distance against velocity
	float dynamicScaling = 0.1f;

	//scaling acceleration output against velocity
	float heavyDynamicScaling = 0.5f;
	float heavyDynamicOffset = 0.5f;

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