#pragma once
#include "BehaviourTree.h"



/*
* class BehaviourDecorator
* child class of Behaviour
*
* a branch node on a behaviour tree, modifies the execution
* of a single leaf node below it using pre an post processing functions
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class BehaviourDecorator : public Behaviour
{

public:

	//pointer to the tree that holds this behaviour
	BehaviourTree* tree = nullptr;

	//pointer to the child that gets updated by this decorator
	Behaviour* child = nullptr;

	//pointer to the function which executes before calling children
	void(*preProcess)(BehaviourDecorator* self, Agent* agent) = nullptr;

	//pointer to the function which executes after calling children
	BehaviourResponse(*postProcess)(BehaviourDecorator* self, Agent* agent, BehaviourResponse reponse) = nullptr;

	/*
	* BehaviourDecorator()
	* default contstructor
	*/
	BehaviourDecorator() {};


	/*
	* ~BehaviourDecorator()
	* destructor, deletes the child
	*/
	~BehaviourDecorator();


	/*
	* update
	* overrides Behaviour's update(float deltaTime)
	*
	* runs the action, in most cases should return SUCCESS
	* can return PENDING, which means that the action is yet
	* to complete it's task and the tree must wait 1 frame before
	* checking for a SUCCESS
	*
	* @param float deltaTime - the amount of time passed since the last frame
	* @returns BehaviourResponse - the response from the behaviour
	*/
	BehaviourResponse update(float deltaTime) override;

};