#include "BehaviourDecorator.h"

//destructor, deletes the child
BehaviourDecorator::~BehaviourDecorator()
{
	//check if a child pointer has been properly assigned yet
	if (child != nullptr)
	{
		delete child;
	}
}

//executes it's functions and children's functions
BehaviourResponse BehaviourDecorator::update(float deltaTime)
{
	//run the pre-process function
	preProcess(this, tree->agent);

	//run the child's update function
	BehaviourResponse response = child->update(deltaTime);

	//check if the child's update is pending
	if (response == BehaviourResponse::PENDING)
	{
		return BehaviourResponse::PENDING;
	}

	//run the post-process function
	return postProcess(this, tree->agent, response);
}
