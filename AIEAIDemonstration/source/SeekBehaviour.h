#pragma once
#include "SteeringBehaviour.h"


/*
* class SeekBehaviour
*
* a steering behaviour which steers toward a static target
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class SeekBehaviour : public SteeringBehaviour
{
public:

	//pointer to the vector to navigate towards
	Vector2* target = nullptr;

	/*
	* SeekBehaviour()
	* default constructor
	*/
	SeekBehaviour() {}


	/*
	* ~SeekBehaviour()
	* default destructor
	*/
	~SeekBehaviour() {}


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