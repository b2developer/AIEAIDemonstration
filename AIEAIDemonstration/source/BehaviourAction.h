#pragma once
#include "BehaviourTree.h"



/*
* class BehaviourAction
* child class of Behaviour
*
* a leaf node on a behaviour tree, action behaviours
* are what make the tree interact with the enviroment
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class BehaviourAction : public Behaviour
{

public:

	//pointer to the tree that holds this behaviour
	BehaviourTree* tree = nullptr;

	//pointer to the function which executes the action
	BehaviourResponse(*execution)(Agent* agent) = nullptr;

	/*
	* BehaviourAction()
	* default contstructor
	*/
	BehaviourAction() {};


	/*
	* ~BehaviourAction()
	* default destructor
	*/
	~BehaviourAction() {};


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