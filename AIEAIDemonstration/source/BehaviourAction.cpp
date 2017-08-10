#include "BehaviourAction.h"

//executes the action of the behaviour
BehaviourResponse BehaviourAction::update(float deltaTime)
{
	//execute the action of the behaviour tree
	return execution(tree->agent);
}
