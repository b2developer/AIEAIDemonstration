#pragma once
#include "SteeringBehaviour.h"


/*
* class FleeBehaviour
*
* a steering behaviour which steers away from a static target
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class FleeBehaviour : public SteeringBehaviour
{
public:

	//pointer to the vector to navigate away from
	Vector2* target = nullptr;

	/*
	* FleeBehaviour()
	* default constructor
	*/
	FleeBehaviour() {}


	/*
	* ~FleeBehaviour()
	* default destructor
	*/
	~FleeBehaviour() {}


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