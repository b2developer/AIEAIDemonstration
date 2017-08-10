#pragma once
#include "BehaviourTree.h"



/*
* class BehaviourCondition
* child class of Behaviour
*
* a leaf node on a behaviour tree, evaluates data and
* returns success or failure depending on the condition
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class BehaviourCondition : public Behaviour
{

public:

	//pointer to the tree that holds this behaviour
	BehaviourTree* tree = nullptr;

	//pointer to the function which evaluates the condition
	BehaviourResponse(*evaluation)(Agent* agent) = nullptr;

	/*
	* BehaviourCondition()
	* default contstructor
	*/
	BehaviourCondition() {};


	/*
	* ~BehaviourCondition()
	* default destructor
	*/
	~BehaviourCondition() {};


	/*
	* update
	* overrides Behaviour's update(float deltaTime)
	*
	* checks the condition, returns SUCCESS if the condition is true,
	* FAILURE if it is false
	*
	* @param float deltaTime - the amount of time passed since the last frame
	* @returns BehaviourResponse - the response from the behaviour
	*/
	BehaviourResponse update(float deltaTime) override;

};