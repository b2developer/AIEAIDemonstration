#pragma once
#include "BehaviourTree.h"
#include <vector>



/*
* class BehaviourSequence
* child class of Behaviour
*
* a branch node of a behaviour tree that resembles an AND gate
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class BehaviourSequence : public Behaviour
{

public:

	//pointer to the tree that holds this behaviour
	BehaviourTree* tree = nullptr;

	//list of behaviour pointers
	std::vector<Behaviour*> children;

	//index of the pending child
	size_t pendingI = 0;

	/*
	* BehaviourSequence()
	* default contstructor
	*/
	BehaviourSequence() {};


	/*
	* ~BehaviourSequence()
	* destructor, deletes all children
	*/
	~BehaviourSequence();


	/*
	* update
	* overrides Behaviour's update(float deltaTime)
	*
	* runs all children, stops evaluating children if one evaluates
	* to FAILURE and returns FAILURE if this occurs, evaluates to PENDING
	* if at least one of it's children evaluates to PENDING. updating
	* this node after a PENDING response will cause the node to update
	* the same behavior that was PENDING repeatedly until it evaluates to
	* something else
	*
	* @param float deltaTime - the amount of time passed since the last frame
	* @returns BehaviourResponse - the response from the behaviour
	*/
	BehaviourResponse update(float deltaTime) override;

};