#include "BehaviourSequence.h"

//destructor, deletes all children
BehaviourSequence::~BehaviourSequence()
{
	//iterate through all children, deleting them all
	for (size_t i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
}

//executes all children until a FAILURE occurs
BehaviourResponse BehaviourSequence::update(float deltaTime)
{

	//iterate through all children, updating them and checking for a pending condition
	for (size_t i = pendingI; i < children.size(); i++)
	{
		//get the response from the child
		BehaviourResponse response = children[i]->update(deltaTime);

		if (response == BehaviourResponse::FAILURE)
		{
			//exit without updating other behaviours
			pendingI = 0;
			return BehaviourResponse::FAILURE;
		}
		else if (response == BehaviourResponse::PENDING)
		{
			//exit, the behaviour is still updating
			pendingI = i;

			return BehaviourResponse::PENDING;
		}
	}

	//no behaviours returned false, return a SUCCESS
	pendingI = 0;
	return BehaviourResponse::SUCCESS;
}
